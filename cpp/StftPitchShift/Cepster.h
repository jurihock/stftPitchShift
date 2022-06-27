#pragma once

#include <cassert>
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
      cutoff = static_cast<size_t>(quefrency * samplerate);
    }

    void lifter(std::vector<T>& envelope)
    {
      assert(envelope.size() == spectrum.size());

      for (size_t i = 0; i < envelope.size(); ++i)
      {
        const T value = envelope[i];

        spectrum[i] = value ? std::log10(value) : -12;
      }

      fft->ifft(spectrum, cepstrum);
      lowpass(cepstrum, cutoff);
      fft->fft(cepstrum, spectrum);

      for (size_t i = 0; i < envelope.size(); ++i)
      {
        const T value = spectrum[i].real();

        envelope[i] = std::pow(T(10), value);
      }
    }

  private:

    const std::shared_ptr<FFT> fft;
    const size_t framesize;
    const double samplerate;

    double value;
    size_t cutoff;

    std::vector<std::complex<T>> spectrum;
    std::vector<T> cepstrum;

    static void lowpass(std::vector<T>& cepstrum, const size_t cutoff)
    {
      for (size_t i = 1; i < std::min(cutoff, cepstrum.size()); ++i)
      {
        cepstrum[i] *= 2;
      }

      for (size_t i = cutoff + 1; i < cepstrum.size(); ++i)
      {
        cepstrum[i] = 0;
      }
    }

  };
}
