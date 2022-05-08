#pragma once

#include <cmath>
#include <complex>
#include <numeric>
#include <vector>

template<class T>
class Normalizer
{

public:

  Normalizer()
  {
  }

  void calibrate(const std::vector<std::complex<T>>& data)
  {
    target = rms(data);
  }

  void normalize(std::vector<std::complex<T>>& data) const
  {
    const T a = target;
    const T b = rms(data);

    if (b == 0)
    {
      return;
    }

    const T c = std::sqrt(a / b);

    for (size_t i = 0; i < data.size(); ++i)
    {
      data[i].real(data[i].real() * c);
    }
  }

private:

  T target;

  static T rms(const std::vector<std::complex<T>>& data)
  {
    // without 1/N and sqrt

    return std::accumulate(
      data.begin(),
      data.end(),
      T(0),
      [](const T result, const std::complex<T>& value)
      {
        return result + value.real() * value.real();
      });
  }

};
