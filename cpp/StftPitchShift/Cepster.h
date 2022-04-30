#pragma once

#include <cmath>
#include <complex>
#include <vector>

#include <StftPitchShift/FFT.h>

class Cepster
{

public:

  Cepster(const std::shared_ptr<FFT> fft, const double quefrency, const double samplerate);

  void lifter(const std::vector<std::complex<float>>& dft, std::vector<float>& envelope);

private:

  const std::shared_ptr<FFT> fft;
  const size_t quefrency;

  std::vector<std::complex<float>> spectrum;
  std::vector<float> cepstrum;

  static void lowpass(std::vector<float>& cepstrum, const size_t quefrency);

};
