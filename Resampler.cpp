#include <Resampler.h>

Resampler::Resampler(const double factor) :
  factor(factor)
{
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

    const float i = (n - n0) / (n1 - n0);

    const std::complex<float> y0 = x[static_cast<size_t>(n0)];
    const std::complex<float> y1 = x[static_cast<size_t>(n1)];

    y[m].real(y0.real() * (1 - i) + y1.real() * i);
    y[m].imag(y0.imag() * (1 - i) + y1.imag() * i);
  }
}
