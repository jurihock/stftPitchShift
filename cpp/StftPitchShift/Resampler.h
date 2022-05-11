#pragma once

#include <cassert>
#include <cmath>
#include <complex>
#include <vector>

namespace stftpitchshift
{
  template<class T>
  class Resampler
  {

  public:

    Resampler()
    {
    }

    double factor() const
    {
      return value;
    }

    void factor(const double factor)
    {
      value = factor;
    }

    void linear(const std::vector<std::complex<T>>& x,
                std::vector<std::complex<T>>& y) const
    {
      assert(x.size() == y.size());

      const ptrdiff_t n = static_cast<ptrdiff_t>(x.size());
      const ptrdiff_t m = static_cast<ptrdiff_t>(n * value);

      const T q = T(n) / T(m);

      const auto interpolate = [&](const ptrdiff_t i)
      {
        T k = i * q;

        const ptrdiff_t j = static_cast<ptrdiff_t>(std::trunc(k));

        k = k - j;

        const bool ok = (0 <= j) && (j < n - 1);

        if (!ok)
        {
          return;
        }

        // TODO cosine interpolation
        // k = T(0.5) - T(0.5) * std::cos(k * std::acos(T(-1)));

        y[i] = k * x[j + 1] + (1 - k) * x[j];
      };

      if (value < 1)
      {
        for (ptrdiff_t i = 0; i < std::min(n, m); ++i)
        {
          interpolate(i);
        }
      }
      else if (value > 1)
      {
        for (ptrdiff_t i = std::min(n, m) - 1; i >= 0; --i)
        {
          interpolate(i);
        }
      }
      else
      {
        y = x;
      }
    }

    void bilinear(const std::vector<std::complex<T>>& x0,
                  const std::vector<std::complex<T>>& x1,
                  std::vector<std::complex<T>>& y) const
    {
      assert(x0.size() == y.size());
      assert(x1.size() == y.size());

      std::vector<std::complex<T>> y0(x0.size());
      std::vector<std::complex<T>> y1(x1.size());

      linear(x0, y0);
      linear(x1, y1);

      for (size_t i = 0; i < y.size(); ++i)
      {
        y[i] = (y0[i] + y1[i]) / T(2);
      }
    }

  private:

    double value;

  };
}
