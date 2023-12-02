#pragma once

#include <StftPitchShift/FFT.h>
#include <StftPitchShift/RFFT.h>

#include <StftPitchShift/Normalizer.h>
#include <StftPitchShift/Vocoder.h>
#include <StftPitchShift/Pitcher.h>
#include <StftPitchShift/Cepster.h>
#include <StftPitchShift/Resampler.h>

namespace stftpitchshift
{
  template<class T>
  class StftPitchShiftCore
  {

  public:

    StftPitchShiftCore(
      const double samplerate,
      const size_t framesize,
      const size_t hopsize) :
      StftPitchShiftCore(
        std::make_shared<RFFT>(),
        samplerate,
        std::make_tuple(framesize, framesize),
        hopsize)
    {
    }

    StftPitchShiftCore(
      const double samplerate,
      const std::tuple<size_t, size_t> framesize,
      const size_t hopsize) :
      StftPitchShiftCore(
        std::make_shared<RFFT>(),
        samplerate,
        framesize,
        hopsize)
    {
    }

    StftPitchShiftCore(
      const std::shared_ptr<FFT> fft,
      const double samplerate,
      const size_t framesize,
      const size_t hopsize) :
      StftPitchShiftCore(
        fft,
        samplerate,
        std::make_tuple(framesize, framesize),
        hopsize)
    {
    }

    StftPitchShiftCore(
      const std::shared_ptr<FFT> fft,
      const double samplerate,
      const std::tuple<size_t, size_t> framesize,
      const size_t hopsize) :
      fft(fft),
      samplerate(samplerate),
      framesize(framesize),
      hopsize(hopsize),
      vocoder(samplerate, framesize, hopsize),
      pitcher(samplerate, std::get<0>(framesize)),
      cepster(fft, samplerate, std::get<0>(framesize)),
      envelope(std::get<0>(framesize) / 2 + 1)
    {
    }

    const std::vector<double>& factors() const
    {
      return pitcher.factors();
    }

    void factors(const std::vector<double>& factors)
    {
      pitcher.factors(factors);
    }

    double quefrency() const
    {
      return cepster.quefrency();
    }

    void quefrency(const double quefrency)
    {
      cepster.quefrency(quefrency);
    }

    double distortion() const
    {
      return resampler.factor();
    }

    void distortion(const double distortion)
    {
      resampler.factor(distortion);
    }

    bool normalization() const
    {
      return normalizer;
    }

    void normalization(const bool normalization)
    {
      if (normalization)
      {
        normalizer = std::make_shared<Normalizer<T>>();
      }
      else
      {
        normalizer = nullptr;
      }
    }

    void shiftpitch(const std::span<std::complex<T>> dft)
    {
      vocoder.encode(dft);

      if (normalizer)
      {
        normalizer->calibrate(dft);
      }

      if (cepster.quefrency())
      {
        for (size_t i = 0; i < dft.size(); ++i)
        {
          envelope[i] = dft[i].real();
        }

        cepster.lifter(envelope);

        for (size_t i = 0; i < dft.size(); ++i)
        {
          if (std::isnormal(envelope[i]))
          {
            dft[i].real(dft[i].real() / envelope[i]);
          }
          else
          {
            envelope[i] = 0;
            dft[i].real(0);
          }
        }

        resampler.linear(envelope);
        pitcher.shiftpitch(dft);

        for (size_t i = 0; i < dft.size(); ++i)
        {
          dft[i].real(dft[i].real() * envelope[i]);
        }
      }
      else
      {
        pitcher.shiftpitch(dft);
      }

      if (normalizer)
      {
        normalizer->normalize(dft);
      }

      vocoder.decode(dft);
    }

  private:

    const std::shared_ptr<FFT> fft;
    const double samplerate;
    const std::tuple<size_t, size_t> framesize;
    const size_t hopsize;

    Vocoder<T> vocoder;
    Pitcher<T> pitcher;
    Cepster<T> cepster;
    Resampler<T> resampler;

    std::vector<T> envelope;

    std::shared_ptr<Normalizer<T>> normalizer;

  };
}
