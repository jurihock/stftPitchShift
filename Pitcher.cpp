#include <Pitcher.h>

Pitcher::Pitcher(const std::vector<float>& factors) :
  factors(factors)
{
}

void Pitcher::shiftpitch(std::vector<std::complex<float>>& dft)
{
  if (prepare)
  {
    resample.resize(factors.size());
    buffer.resize(factors.size());
    mask.resize(dft.size());

    for (size_t i = 0; i < factors.size(); ++i)
    {
      resample[i] = Resampler(factors[i]);
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
      buffer[0][j].imag(buffer[0][j].imag() * factors[0]);
    }

    dft = buffer[0];
    
    return;
  }

  for (size_t i = 0; i < factors.size(); ++i)
  {
    resample[i].linear(dft, buffer[i]);
    
    for (size_t j = 0; j < dft.size(); ++j)
    {
      buffer[i][j].imag(buffer[i][j].imag() * factors[i]);
    }
  }

  for (size_t j = 0; j < dft.size(); ++j)
  {
    float maximum = std::numeric_limits<float>::lowest();

    for (size_t i = 0; i < factors.size(); ++i)
    {
      const float current = buffer[i][j].real();

      if (current > maximum)
      {
        maximum = current;
        mask[j] = i;
      }
    }
  }

  for (size_t j = 0; j < dft.size(); ++j)
  {
    dft[j] = buffer[mask[j]][j];
  }
}
