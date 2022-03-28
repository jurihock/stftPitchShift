#pragma once

#include <complex>
#include <functional>
#include <vector>

class STFT
{

public:

  STFT(const size_t framesize, const size_t hopsize);

  void operator()(const std::vector<float>& input, std::vector<float>& output, const std::function<void(std::vector<std::complex<float>>& dft)> callback) const;

private:

  const size_t framesize;
  const size_t hopsize;

  std::vector<float> window;
  float unitygain;

  static void reject(const std::vector<float>& input, std::vector<float>& frame, const size_t hop);
  static void inject(std::vector<float>& output, const std::vector<float>& frame, const size_t hop);
  static void weight(std::vector<float>& frame, const std::vector<float>& window, const float scale);
  static void fft(const std::vector<float>& frame, std::vector<std::complex<float>>& dft, const float scale);
  static void ifft(const std::vector<std::complex<float>>& dft, std::vector<float>& frame, const float scale);
};