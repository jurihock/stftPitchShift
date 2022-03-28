#pragma once

#include <cmath>
#include <complex>
#include <vector>

class Vocoder
{

public:

  Vocoder(const size_t framesize, const size_t hopsize, const float samplerate);

  void encode(std::vector<std::complex<float>>& dft);
  void decode(std::vector<std::complex<float>>& dft);

private:

  const double PI = std::acos(-1.0);
  const double PI2 = 2.0 * std::acos(-1.0);

  const size_t framesize;
  const size_t hopsize;

  const double stft_freq_inc;
  const double stft_phase_inc;

  std::vector<double> encode_phase_buffer;
  std::vector<double> decode_phase_buffer;

  inline double wrap(const double phase) const
  {
    return phase - PI2 * std::floor(phase / PI2 + 0.5);
  }

};