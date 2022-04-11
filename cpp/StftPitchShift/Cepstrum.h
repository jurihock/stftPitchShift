#pragma once

#include <cmath>
#include <complex>
#include <vector>

class Cepstrum
{

public:

  Cepstrum(const float quefrency, const float samplerate);

  void lifter(const std::vector<std::complex<float>>& dft,
              std::vector<float>& envelope);

private:

  const size_t quefrency;

  std::vector<std::complex<float>> spectrum;
  std::vector<float> cepstrum;

  static void lowpass(std::vector<float>& cepstrum, const size_t quefrency);

  static void fft(const std::vector<float>& frame, std::vector<std::complex<float>>& dft);
  static void ifft(const std::vector<std::complex<float>>& dft, std::vector<float>& frame);

};
