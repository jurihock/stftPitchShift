#pragma once

#include <cassert>
#include <cmath>
#include <complex>
#include <vector>

namespace stftpitchshift
{
  template<class T>
  class Vocoder
  {

  public:

    Vocoder(const size_t framesize, const size_t hopsize, const double samplerate) :
      stft_freq_inc(samplerate / (double)framesize),
      stft_phase_inc(PI2 / ((double)framesize / (double)hopsize))
    {
      encode_phase_buffer.resize(framesize / 2 + 1);
      decode_phase_buffer.resize(framesize / 2 + 1);
    }

    void encode(std::vector<std::complex<T>>& dft)
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

    void decode(std::vector<std::complex<T>>& dft)
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

        dft[i] = std::polar<T>(
          dft[i].real(),
          static_cast<T>(phase));
      }
    }

  private:

    const double PI2 = 2.0 * std::acos(-1.0);

    const double stft_freq_inc;
    const double stft_phase_inc;

    std::vector<double> encode_phase_buffer;
    std::vector<double> decode_phase_buffer;

    inline double wrap(const double phase) const
    {
      return phase - PI2 * std::floor(phase / PI2 + 0.5);
    }

  };
}
