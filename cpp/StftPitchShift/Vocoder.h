#pragma once

#include <cassert>
#include <cmath>
#include <complex>
#include <span>
#include <tuple>
#include <vector>

namespace stftpitchshift
{
  template<class T>
  class Vocoder
  {

  public:

    Vocoder(const size_t framesize, const size_t hopsize, const double samplerate) :
      Vocoder(std::make_tuple(framesize, framesize), hopsize, samplerate)
    {
    }

    Vocoder(const std::tuple<size_t, size_t> framesize, const size_t hopsize, const double samplerate)
    {
      const size_t dftsize = std::get<0>(framesize) / 2 + 1;

      stft_freq_inc = samplerate / std::get<0>(framesize);
      stft_phase_inc = pi * hopsize / std::get<0>(framesize);

      encode_phase_buffer.resize(dftsize);
      decode_phase_buffer.resize(dftsize);
      decode_phase_shift.resize(dftsize);

      if (std::get<1>(framesize) != std::get<0>(framesize))
      {
        for (size_t i = 0; i < dftsize; ++i)
        {
          // compensate asymmetric synthesis window by virtual time shifting #38
          decode_phase_shift[i] = pi * i * std::get<1>(framesize) / dftsize;
        }
      }
    }

    void encode(const std::span<std::complex<T>> dft)
    {
      assert(dft.size() == encode_phase_buffer.size());

      double frequency,
             phase,
             delta,
             j;

      for (size_t i = 0; i < dft.size(); ++i)
      {
        phase = std::arg(dft[i]);

        delta = phase - encode_phase_buffer[i];
        encode_phase_buffer[i] = phase;

        j = wrap(delta - i * stft_phase_inc) / stft_phase_inc;

        frequency = (i + j) * stft_freq_inc;

        dft[i] = std::complex<T>(
          std::abs(dft[i]),
          static_cast<T>(frequency));
      }
    }

    void decode(const std::span<std::complex<T>> dft)
    {
      assert(dft.size() == decode_phase_buffer.size());

      double frequency,
             phase,
             delta,
             j;

      for (size_t i = 0; i < dft.size(); ++i)
      {
        frequency = dft[i].imag();

        j = (frequency - i * stft_freq_inc) / stft_freq_inc;

        delta = (i + j) * stft_phase_inc;

        decode_phase_buffer[i] += delta;
        phase = decode_phase_buffer[i];

        phase -= decode_phase_shift[i]; // #38

        dft[i] = std::polar<T>(
          dft[i].real(),
          static_cast<T>(phase));
      }
    }

  private:

    const double pi = 2.0 * std::acos(-1.0);

    double stft_freq_inc;
    double stft_phase_inc;

    std::vector<double> encode_phase_buffer;
    std::vector<double> decode_phase_buffer;
    std::vector<double> decode_phase_shift;

    inline double wrap(const double phase) const
    {
      return phase - pi * std::floor(phase / pi + 0.5);
    }

  };
}
