#pragma once

#include <cmath>
#include <complex>
#include <limits>
#include <vector>

#include <StftPitchShift/Resampler.h>

class Pitcher
{

public:

  Pitcher(const std::vector<double>& factors);

  void shiftpitch(std::vector<std::complex<float>>& dft);

private:

  const std::vector<double> factors;

  std::vector<Resampler<float>> resample;
  std::vector<std::vector<std::complex<float>>> buffer;

  bool prepare = true;

};
