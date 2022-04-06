#pragma once

#include <algorithm>
#include <cmath>
#include <complex>
#include <limits>
#include <map>
#include <vector>

#include <Resampler.h>

class Pitcher
{

public:

  Pitcher(const std::vector<float>& factors);

  void shiftpitch(std::vector<std::complex<float>>& dft);

private:

  const std::vector<float> factors;

  std::map<float, Resampler> resample;
  std::map<float, std::vector<std::complex<float>>> buffer;
  std::vector<float> mask;

  bool prepare = true;

};
