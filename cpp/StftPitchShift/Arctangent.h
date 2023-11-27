#pragma once

#include <cmath>
#include <complex>
#include <concepts>

namespace stftpitchshift
{
  /**
   * Arctangent approximation according to [1].
   *
   * [1] Xavier Girones and Carme Julia and Domenec Puig
   *     Full Quadrant Approximations for the Arctangent Function
   *     IEEE Signal Processing Magazine (2013)
   *     https://ieeexplore.ieee.org/document/6375931
   **/
  namespace Arctangent
  {
    template<std::floating_point T>
    inline static T atan2(const T y, const T x)
    {
      if (y == 0 && x == 0)
      {
        return T(0);
      }

      const int ys = std::signbit(y);
      const int xs = std::signbit(x);

      const int q = ((ys & ~xs) << 2) | (xs << 1);
      const int s = (ys ^ xs) ? -1 : +1;

      const T yx = y * x;
      const T yy = y * y;
      const T xx = x * x;

      const T a = T(0.596227);
      const T b = std::abs(a * yx);
      const T c = b + yy;
      const T d = c / (c + b + xx);
      const T e = q + std::copysign(d, s);
      const T f = T(1.57079632679489661923);

      return e * f;
    }

    template<std::floating_point T>
    inline static T atan2(const std::complex<T>& z)
    {
      return Arctangent::atan2(z.imag(), z.real());
    }
  }
}
