#pragma once

#include <cmath>
#include <complex>
#include <limits>
#include <vector>

#include <StftPitchShift/Resampler.h>

template<class T>
class Pitcher
{

public:

  Pitcher(const std::vector<double>& factors) :
    factors(factors)
  {
  }

  void shiftpitch(std::vector<std::complex<T>>& dft)
  {
    if (prepare)
    {
      resample.resize(factors.size());
      buffer.resize(factors.size());

      for (size_t i = 0; i < factors.size(); ++i)
      {
        resample[i] = Resampler<T>(factors[i]);
        buffer[i].resize(dft.size());
      }

      prepare = false;
    }

    if (factors.size() == 1)
    {
      if (factors[0] == 1)
      {
        return;
      }

      resample[0].linear(dft, buffer[0]);

      for (size_t j = 0; j < dft.size(); ++j)
      {
        buffer[0][j].imag(buffer[0][j].imag() * T(factors[0]));
      }

      dft = buffer[0];

      return;
    }

    for (size_t i = 0; i < factors.size(); ++i)
    {
      resample[i].linear(dft, buffer[i]);

      for (size_t j = 0; j < dft.size(); ++j)
      {
        buffer[i][j].imag(buffer[i][j].imag() * T(factors[i]));
      }
    }

    for (size_t j = 0; j < dft.size(); ++j)
    {
      T maximum = std::numeric_limits<T>::lowest();
      size_t mask = 0;

      for (size_t i = 0; i < factors.size(); ++i)
      {
        const T current = buffer[i][j].real();

        if (current > maximum)
        {
          maximum = current;
          mask = i;
        }
      }

      dft[j] = buffer[mask][j];
    }
  }

private:

  const std::vector<double> factors;

  std::vector<Resampler<T>> resample;
  std::vector<std::vector<std::complex<T>>> buffer;

  bool prepare = true;

};
