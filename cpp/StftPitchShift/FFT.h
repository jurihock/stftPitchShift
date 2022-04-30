#pragma once

#include <complex>
#include <memory>
#include <vector>

class FFT
{

public:

  virtual ~FFT() {}

  virtual void fft(const std::vector<float>& frame, std::vector<std::complex<float>>& dft) = 0;
  virtual void fft(const std::vector<double>& frame, std::vector<std::complex<double>>& dft) = 0;

  virtual void ifft(const std::vector<std::complex<float>>& dft, std::vector<float>& frame) = 0;
  virtual void ifft(const std::vector<std::complex<double>>& dft, std::vector<double>& frame) = 0;

};

// ~ DEFAULT FFT BACKEND ~

#include <pocketfft/pocketfft_hdronly.h>

class Pocketfft : public FFT
{

public:

  void fft(const std::vector<float>& frame, std::vector<std::complex<float>>& dft) override
  {
    pocketfft::r2c(
      { frame.size() },
      { sizeof(float) },
      { sizeof(std::complex<float>) },
      0,
      true,
      frame.data(),
      dft.data(),
      1.0f / frame.size());
  }

  void fft(const std::vector<double>& frame, std::vector<std::complex<double>>& dft) override
  {
    pocketfft::r2c(
      { frame.size() },
      { sizeof(double) },
      { sizeof(std::complex<double>) },
      0,
      true,
      frame.data(),
      dft.data(),
      1.0 / frame.size());
  }

  void ifft(const std::vector<std::complex<float>>& dft, std::vector<float>& frame) override
  {
    pocketfft::c2r(
      { frame.size() },
      { sizeof(std::complex<float>) },
      { sizeof(float) },
      0,
      false,
      dft.data(),
      frame.data(),
      1.0f);
  }

  void ifft(const std::vector<std::complex<double>>& dft, std::vector<double>& frame) override
  {
    pocketfft::c2r(
      { frame.size() },
      { sizeof(std::complex<double>) },
      { sizeof(double) },
      0,
      false,
      dft.data(),
      frame.data(),
      1.0);
  }

};
