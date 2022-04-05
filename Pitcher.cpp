#include <Pitcher.h>

Pitcher::Pitcher(const std::vector<float>& factors) :
  factors(factors)
{
}

void Pitcher::shiftpitch(std::vector<std::complex<float>>& frame)
{
  if (prepare)
  {
    for (float factor : factors)
    {
      resample[factor] = Resampler(factor);
      buffer[factor].resize(frame.size());
    }

    mask.resize(frame.size());

    prepare = false;
  }

  for (float factor : factors)
  {
    resample[factor].linear(frame, buffer[factor]);

    for (size_t i = 0; i < frame.size(); ++i)
    {
      buffer[factor][i].imag(buffer[factor][i].imag() * factor);
    }
  }

  if (factors.size() == 1)
  {
    std::copy(
      buffer[factors.front()].begin(),
      buffer[factors.front()].end(),
      frame.begin());

    return;
  }

  for (size_t i = 0; i < frame.size(); ++i)
  {
    float maximum = std::numeric_limits<float>::lowest();

    for (float factor : factors)
    {
      const float current = buffer[factor][i].real();

      if (current > maximum)
      {
        maximum = current;
        mask[i] = factor;
      }
    }
  }

  for (size_t i = 0; i < frame.size(); ++i)
  {
    frame[i] = buffer[mask[i]][i];
  }
}
