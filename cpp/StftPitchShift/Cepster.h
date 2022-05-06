#pragma once

#include <cmath>
#include <complex>
#include <vector>

#include <StftPitchShift/FFT.h>

namespace stftpitchshift
{
  template<class T>
  class Cepster
  {

  public:

    Cepster(const std::shared_ptr<FFT> fft, const size_t framesize, const double samplerate) :
      fft(fft),
      framesize(framesize),
      samplerate(samplerate),
      spectrum(framesize / 2 + 1),
      cepstrum(framesize)
    {
    }

    double quefrency() const
    {
      return value;
    }

    void quefrency(const double quefrency)
    {
      value = quefrency;
      threshold = static_cast<size_t>(quefrency * samplerate);
    }

    void lifter(std::vector<T>& envelope)
    {
      for (size_t i = 0; i < spectrum.size(); ++i)
      {
        spectrum[i] = std::log10(envelope[i]);
      }

      fft->ifft(spectrum, cepstrum);
      lowpass(cepstrum, threshold);
      fft->fft(cepstrum, spectrum);

      for (size_t i = 0; i < spectrum.size(); ++i)
      {
        envelope[i] = std::pow(T(10), spectrum[i].real());
      }
    }

  private:

    const std::shared_ptr<FFT> fft;
    const size_t framesize;
    const double samplerate;

    double value;
    size_t threshold;

    std::vector<std::complex<T>> spectrum;
    std::vector<T> cepstrum;

    static void lowpass(std::vector<T>& cepstrum, const size_t threshold)
    {
      for (size_t i = 1; i < std::min(threshold, cepstrum.size()); ++i)
      {
        cepstrum[i] *= 2;
      }

      for (size_t i = threshold + 1; i < cepstrum.size(); ++i)
      {
        cepstrum[i] = 0;
      }
    }

  };
}
