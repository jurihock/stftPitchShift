#pragma once

#include <cmath>
#include <complex>
#include <vector>

#include <StftPitchShift/FFT.h>

class Cepstrum
{

public:

  Cepstrum(const std::shared_ptr<FFT> fft, const float quefrency, const float samplerate);

  void lifter(const std::vector<std::complex<float>>& dft, std::vector<float>& envelope);

private:

  const std::shared_ptr<FFT> fft;
  const size_t quefrency;

  std::vector<std::complex<float>> spectrum;
  std::vector<float> cepstrum;

  static void lowpass(std::vector<float>& cepstrum, const size_t quefrency);

};
