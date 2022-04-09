#pragma once

#include <cmath>
#include <complex>
#include <functional>
#include <vector>

class STFT
{

public:

  STFT(const size_t framesize, const size_t hopsize);

  void operator()(const std::vector<float>& input, std::vector<float>& output, const std::function<void(std::vector<std::complex<float>>& dft)> callback) const;
  void operator()(const size_t size, const float* input, float* const output, const std::function<void(std::vector<std::complex<float>>& dft)> callback) const;

private:

  const float PI2 = 2.0f * std::acos(-1.0f);

  const size_t framesize;
  const size_t hopsize;

  std::vector<float> window;
  float unitygain;

  static void reject(const size_t size, const float* input, std::vector<float>& frame, const size_t hop);
  static void inject(const size_t size, float* const output, const std::vector<float>& frame, const size_t hop);
  static void weight(std::vector<float>& frame, const std::vector<float>& window, const float scale);
  static void fft(const std::vector<float>& frame, std::vector<std::complex<float>>& dft, const float scale);
  static void ifft(const std::vector<std::complex<float>>& dft, std::vector<float>& frame, const float scale);

};
