#pragma once

#include <cmath>
#include <complex>
#include <limits>
#include <vector>

#include <StftPitchShift/Resampler.h>

namespace stftpitchshift
{
  template<class T>
  class Pitcher
  {

  public:

    Pitcher(const size_t framesize, const double samplerate) :
      framesize(framesize),
      samplerate(samplerate),
      nyquist(samplerate / 2)
    {
    }

    const std::vector<double>& factors() const
    {
      return values;
    }

    void factors(const std::vector<double>& factors)
    {
      values = factors;

      resample.resize(values.size());
      buffer.resize(values.size());

      for (size_t i = 0; i < values.size(); ++i)
      {
        resample[i].factor(values[i]);
        buffer[i].resize(framesize / 2 + 1);
      }
    }

    void shiftpitch(std::vector<std::complex<T>>& dft)
    {
      if (values.empty())
      {
        return;
      }

      if (values.size() == 1)
      {
        if (values[0] == 1)
        {
          return;
        }

        resample[0].linear(dft, buffer[0]);

        for (size_t j = 0; j < dft.size(); ++j)
        {
          const T frequency = buffer[0][j].imag() * T(values[0]);

          buffer[0][j].imag(frequency);

          if (frequency <= 0 || frequency >= nyquist)
          {
            buffer[0][j].real(0);
          }
        }

        dft = buffer[0];

        return;
      }

      for (size_t i = 0; i < values.size(); ++i)
      {
        resample[i].linear(dft, buffer[i]);

        for (size_t j = 0; j < dft.size(); ++j)
        {
          const T frequency = buffer[i][j].imag() * T(values[i]);

          buffer[i][j].imag(frequency);

          if (frequency <= 0 || frequency >= nyquist)
          {
            buffer[i][j].real(0);
          }
        }
      }

      for (size_t j = 0; j < dft.size(); ++j)
      {
        T maximum = std::numeric_limits<T>::lowest();
        size_t mask = 0;

        for (size_t i = 0; i < values.size(); ++i)
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

    const size_t framesize;
    const double samplerate;
    const double nyquist;

    std::vector<double> values;

    std::vector<Resampler<T>> resample;
    std::vector<std::vector<std::complex<T>>> buffer;

  };
}
