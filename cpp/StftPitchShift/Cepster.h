#pragma once

#include <cmath>
#include <complex>
#include <vector>

#include <StftPitchShift/FFT.h>

template<class T>
class Cepster
{

public:

  Cepster(const std::shared_ptr<FFT> fft, const double quefrency, const double samplerate) :
    fft(fft),
    quefrency(static_cast<size_t>(quefrency * samplerate))
  {
  }

  void lifter(const std::vector<std::complex<T>>& dft, std::vector<T>& envelope)
  {
    spectrum.resize(dft.size());
    cepstrum.resize((dft.size() - 1) * 2);
    envelope.resize(dft.size());

    for (size_t i = 0; i < spectrum.size(); ++i)
    {
      spectrum[i] = std::log10(dft[i].real());
    }

    fft->ifft(spectrum, cepstrum);
    lowpass(cepstrum, quefrency);
    fft->fft(cepstrum, spectrum);

    for (size_t i = 0; i < spectrum.size(); ++i)
    {
      envelope[i] = std::pow(T(10), spectrum[i].real());
    }
  }

private:

  const std::shared_ptr<FFT> fft;
  const size_t quefrency;

  std::vector<std::complex<T>> spectrum;
  std::vector<T> cepstrum;

  static void lowpass(std::vector<T>& cepstrum, const size_t quefrency)
  {
    for (size_t i = 1; i < std::min(quefrency, cepstrum.size()); ++i)
    {
      cepstrum[i] *= 2;
    }

    for (size_t i = quefrency + 1; i < cepstrum.size(); ++i)
    {
      cepstrum[i] = 0;
    }
  }

};
