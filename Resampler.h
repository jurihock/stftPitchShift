#pragma once

#include <cmath>
#include <complex>
#include <vector>

class Resampler
{

public:

  Resampler(const double factor);

  void linear(const std::vector<std::complex<float>>& x, std::vector<std::complex<float>>& y) const;

private:

  const double factor;

};
