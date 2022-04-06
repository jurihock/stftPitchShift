#pragma once

#include <cmath>
#include <complex>
#include <limits>
#include <vector>

#include <Resampler.h>

class Pitcher
{

public:

  Pitcher(const std::vector<float>& factors);

  void shiftpitch(std::vector<std::complex<float>>& dft);

private:

  const std::vector<float> factors;

  std::vector<Resampler> resample;
  std::vector<std::vector<std::complex<float>>> buffer;
  std::vector<size_t> mask;

  bool prepare = true;

};
