#pragma once

#include <cmath>
#include <complex>
#include <functional>
#include <numeric>
#include <vector>

#include <StftPitchShift/FFT.h>

class STFT
{

public:

  STFT(const std::shared_ptr<FFT> fft, const size_t framesize, const size_t hopsize, const bool chronometry = false);

  void operator()(const std::vector<float>& input, std::vector<float>& output, const std::function<void(std::vector<std::complex<float>>& dft)> callback) const;
  void operator()(const size_t size, const float* input, float* const output, const std::function<void(std::vector<std::complex<float>>& dft)> callback) const;

private:

  const float PI2 = 2.0f * std::acos(-1.0f);

  const std::shared_ptr<FFT> fft;
  const size_t framesize;
  const size_t hopsize;

  const bool chronometry;

  struct
  {
    std::vector<float> analysis;
    std::vector<float> synthesis;
  }
  windows;

  static void reject(const size_t hop, const float* input, std::vector<float>& frame, const std::vector<float>& window);
  static void inject(const size_t hop, float* const output, const std::vector<float>& frame, const std::vector<float>& window);

};
