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

    Resampler() :
      value(1)
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

    void linear(std::vector<T>& x) const
    {
      linear<T>(x, x);
    }

    void linear(const std::vector<T>& x,
                std::vector<T>& y) const
    {
      linear<T>(x, y);
    }

    void linear(std::vector<std::complex<T>>& x) const
    {
      linear<std::complex<T>>(x, x);
    }

    void linear(const std::vector<std::complex<T>>& x,
                std::vector<std::complex<T>>& y) const
    {
      linear<std::complex<T>>(x, y);
    }

    void bilinear(const std::vector<T>& x0,
                  const std::vector<T>& x1,
                  std::vector<T>& y) const
    {
      assert(x0.size() == y.size());
      assert(x1.size() == y.size());

      std::vector<T> y0(x0.size());
      std::vector<T> y1(x1.size());

      linear(x0, y0);
      linear(x1, y1);

      for (size_t i = 0; i < y.size(); ++i)
      {
        y[i] = (y0[i] + y1[i]) / T(2);
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

    template<class V>
    void linear(const std::vector<V>& x,
                std::vector<V>& y) const
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
        assert(m < n);

        for (ptrdiff_t i = 0; i < m; ++i)
        {
          interpolate(i);
        }

        for (ptrdiff_t i = m; i < n; ++i)
        {
          y[i] = 0;
        }
      }
      else if (value > 1)
      {
        assert(m > n);

        for (ptrdiff_t i = n - 1; i >= 0; --i)
        {
          interpolate(i);
        }
      }
      else if (x.data() != y.data())
      {
        y = x;
      }
    }

  };
}
