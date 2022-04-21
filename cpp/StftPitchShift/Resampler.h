#pragma once

#include <algorithm>
#include <cmath>
#include <complex>
#include <vector>

class Resampler
{

public:

  Resampler(const double factor = 1.0);
  Resampler(const Resampler& other);
  
  Resampler& operator=(const Resampler& other);

  void cosine(const std::vector<std::complex<float>>& x,
              std::vector<std::complex<float>>& y) const;

  void linear(const std::vector<std::complex<float>>& x,
              std::vector<std::complex<float>>& y) const;

  void bilinear(const std::vector<std::complex<float>>& x0,
                const std::vector<std::complex<float>>& x1,
                std::vector<std::complex<float>>& y) const;

private:

  const float PI = std::acos(-1.0f);

  double factor;

};
