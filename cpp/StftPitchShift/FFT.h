#pragma once

#include <complex>
#include <memory>
#include <vector>

namespace stftpitchshift
{
  class FFT
  {

  public:

    virtual ~FFT() {}

    virtual void fft(const std::vector<float>& frame, std::vector<std::complex<float>>& dft) = 0;
    virtual void fft(const std::vector<double>& frame, std::vector<std::complex<double>>& dft) = 0;

    virtual void ifft(const std::vector<std::complex<float>>& dft, std::vector<float>& frame) = 0;
    virtual void ifft(const std::vector<std::complex<double>>& dft, std::vector<double>& frame) = 0;

  };
}
