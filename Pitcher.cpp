#include <Pitcher.h>

Pitcher::Pitcher(const std::vector<float>& factors) :
  factors(factors)
{
}

void Pitcher::shiftpitch(std::vector<std::complex<float>>& dft)
{
  if (prepare)
  {
    for (const float factor : factors)
    {
      resample[factor] = Resampler(factor);
      buffer[factor].resize(dft.size());
    }

    mask.resize(dft.size());

    prepare = false;
  }

  if (factors.size() == 1)
  {
    const float factor = factors.front();

    if (factor == 1)
    {
      return;
    }

    resample[factor].linear(dft, buffer[factor]);

    for (size_t i = 0; i < dft.size(); ++i)
    {
      buffer[factor][i].imag(buffer[factor][i].imag() * factor);
    }
    
    dft = buffer[factor];
    
    return;
  }

  for (const float factor : factors)
  {
    resample[factor].linear(dft, buffer[factor]);

    for (size_t i = 0; i < dft.size(); ++i)
    {
      buffer[factor][i].imag(buffer[factor][i].imag() * factor);
    }
  }

  for (size_t i = 0; i < dft.size(); ++i)
  {
    float maximum = std::numeric_limits<float>::lowest();

    for (const float factor : factors)
    {
      const float current = buffer[factor][i].real();

      if (current > maximum)
      {
        maximum = current;
        mask[i] = factor;
      }
    }
  }

  for (size_t i = 0; i < dft.size(); ++i)
  {
    dft[i] = buffer[mask[i]][i];
  }
}
