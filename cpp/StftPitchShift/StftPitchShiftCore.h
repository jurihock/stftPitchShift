#pragma once

#include <StftPitchShift/FFT.h>
#include <StftPitchShift/RFFT.h>

#include <StftPitchShift/Normalizer.h>
#include <StftPitchShift/Vocoder.h>
#include <StftPitchShift/Pitcher.h>
#include <StftPitchShift/Cepster.h>

namespace stftpitchshift
{
  template<class T>
  class StftPitchShiftCore
  {

  public:

    StftPitchShiftCore(
      const size_t framesize,
      const size_t hopsize,
      const double samplerate) :
      StftPitchShiftCore(
        std::make_shared<RFFT>(),
        framesize,
        hopsize,
        samplerate)
    {
    }

    StftPitchShiftCore(
      const std::shared_ptr<FFT> fft,
      const size_t framesize,
      const size_t hopsize,
      const double samplerate) :
      fft(fft),
      framesize(framesize),
      hopsize(hopsize),
      samplerate(samplerate),
      vocoder(framesize, hopsize, samplerate),
      pitcher(framesize),
      cepster(fft, framesize, samplerate),
      envelope(framesize / 2 + 1)
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

    void shiftpitch(std::vector<std::complex<T>>& dft)
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
          const bool ok = std::isnormal(envelope[i]);

          dft[i].real(ok ? dft[i].real() / envelope[i] : 0);
        }

        pitcher.shiftpitch(dft);

        for (size_t i = 0; i < dft.size(); ++i)
        {
          const bool ok = std::isnormal(envelope[i]);

          dft[i].real(ok ? dft[i].real() * envelope[i] : 0);
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
    const size_t framesize;
    const size_t hopsize;
    const double samplerate;

    Vocoder<T> vocoder;
    Pitcher<T> pitcher;
    Cepster<T> cepster;

    std::vector<T> envelope;

    std::shared_ptr<Normalizer<T>> normalizer;

  };
}
