#include <Resampler.h>

Resampler::Resampler(const double factor) :
  factor(factor)
{
}

Resampler::Resampler(const Resampler& other) :
  factor(other.factor)
{
}

Resampler& Resampler::operator=(const Resampler& other)
{
  if(this != &other)
  {
    this->factor = other.factor;
  }

  return *this;
}

void Resampler::linear(const std::vector<std::complex<float>>& x, std::vector<std::complex<float>>& y) const
{
  const size_t N = x.size();
  const size_t M = static_cast<size_t>(std::round(N * factor));

  for (size_t m = 0; m < std::min(M, N); ++m)
  {
    const double n = m / factor;

    const double n0 = std::floor(n);
    const double n1 = std::ceil(n);

    if (n0 < 0 || N <= n0)
    {
      continue;
    }

    if (n1 < 0 || N <= n1)
    {
      continue;
    }

    if (n0 == n1)
    {
      y[m] = x[n0];
      continue;
    }

    const std::complex<float> y0 = x[static_cast<size_t>(n0)];
    const std::complex<float> y1 = x[static_cast<size_t>(n1)];

    const float i = (n - n0) / (n1 - n0);

    y[m] = y0 * (1 - i) + y1 * i;
  }
}

void Resampler::bilinear(const std::vector<std::complex<float>>& x0, const std::vector<std::complex<float>>& x1, std::vector<std::complex<float>>& y) const
{
  std::vector<std::complex<float>> y0(x0.size());
  std::vector<std::complex<float>> y1(x1.size());

  linear(x0, y0);
  linear(x1, y1);

  for (size_t i = 0; i < y.size(); ++i)
  {
    y[i] = (y0[i] + y1[i]) / 2.0f;
  }
}
