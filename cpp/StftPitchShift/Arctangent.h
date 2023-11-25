#pragma once

#include <cmath>

// Avoid M_PI constants and thus the need to
// deal with _USE_MATH_DEFINES in Windows builds.
// https://learn.microsoft.com/cpp/c-runtime-library/math-constants
#define ARCTANGENT_PI_1 3.14159265358979323846
#define ARCTANGENT_PI_2 1.57079632679489661923
#define ARCTANGENT_PI_4 0.785398163397448309616

namespace stftpitchshift
{
  namespace Arctangent
  {
    /**
     * Approximate the arctangent according to [1].
     *
     * [1] Sreeraman Rajan and Sichun Wang and Robert Inkol and Alain Joyal
     *     Efficient approximations for the arctangent function
     *     IEEE Signal Processing Magazine (2006)
     *     https://ieeexplore.ieee.org/document/1628884
     **/
    template<class T>
    inline static T atan(const T q)
    {
      return (T(ARCTANGENT_PI_4 + 0.273) - T(0.273) * q) * q;
    }

    /**
     * Approximate the arctangent according to [1] and [2].
     *
     * [1] Sreeraman Rajan and Sichun Wang and Robert Inkol and Alain Joyal
     *     Efficient approximations for the arctangent function
     *     IEEE Signal Processing Magazine (2006)
     *     https://ieeexplore.ieee.org/document/1628884
     *
     * [2] Dmytro Mishkin
     *     https://github.com/ducha-aiki/fast_atan2
     **/
    template<class T>
    inline static T atan2(const T y, const T x)
    {
      if (y == 0 && x == 0)
      {
        return T(0);
      }

      const auto Y = std::abs(y);
      const auto X = std::abs(x);

      const auto octant = ((x < 0) << 2) + ((y < 0) << 1) + (X <= Y);

      switch (octant)
      {
        /* 1 */ case 0: return + Arctangent::atan(Y / X);
        /* 2 */ case 1: return - Arctangent::atan(X / Y) + T(ARCTANGENT_PI_2);
        /* 3 */ case 5: return + Arctangent::atan(X / Y) + T(ARCTANGENT_PI_2);
        /* 4 */ case 4: return - Arctangent::atan(Y / X) + T(ARCTANGENT_PI_1);
        /* 5 */ case 6: return + Arctangent::atan(Y / X) - T(ARCTANGENT_PI_1);
        /* 6 */ case 7: return - Arctangent::atan(X / Y) - T(ARCTANGENT_PI_2);
        /* 7 */ case 3: return + Arctangent::atan(X / Y) - T(ARCTANGENT_PI_2);
        /* 8 */ case 2: return - Arctangent::atan(Y / X);
      }

      return T(0);
    }
  }
}
