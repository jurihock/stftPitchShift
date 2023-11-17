#pragma once

#include <complex>
#include <span>

namespace stftpitchshift
{
  class FFT
  {

  public:

    virtual ~FFT() {}

    virtual void fft(const std::span<float>  frame, const std::span<std::complex<float>>  dft) = 0;
    virtual void fft(const std::span<double> frame, const std::span<std::complex<double>> dft) = 0;

    virtual void ifft(const std::span<std::complex<float>>  dft, const std::span<float>  frame) = 0;
    virtual void ifft(const std::span<std::complex<double>> dft, const std::span<double> frame) = 0;

  };
}
