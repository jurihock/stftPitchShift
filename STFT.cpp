#include <STFT.h>

#include <cassert>

#include <pocketfft/pocketfft_hdronly.h>

STFT::STFT(const size_t framesize, const size_t hopsize) :
  framesize(framesize),
  hopsize(hopsize)
{
  assert(framesize && !(framesize & (framesize - 1)));

  window.resize(framesize);

  for (size_t i = 0; i < window.size(); ++i)
  {
    window[i] = 0.5f - 0.5f * std::cos(PI2 * i / window.size());
  }

  unitygain = 0;

  for (size_t i = 0; i < window.size(); ++i)
  {
    unitygain += window[i] * window[i];
  }

  unitygain = hopsize / unitygain;
}

void STFT::operator()(const std::vector<float>& input, std::vector<float>& output, const std::function<void(std::vector<std::complex<float>>& dft)> callback) const
{
  std::vector<float> frame(framesize);
  std::vector<std::complex<float>> dft(framesize / 2 + 1);

  const float scalers[4] =
  {
    1.0f,              // analysis window
    1.0f / dft.size(), // forward fft
    1.0f,              // backward fft
    1.0f * unitygain   // synthesis window
  };

  for (size_t hop = 0; (hop + framesize) < input.size(); hop += hopsize)
  {
    reject(input, frame, hop);
    weight(frame, window, scalers[0]);
    fft(frame, dft, scalers[1]);

    callback(dft);

    ifft(dft, frame, scalers[2]);
    weight(frame, window, scalers[3]);
    inject(output, frame, hop);
  }
}

void STFT::reject(const std::vector<float>& input, std::vector<float>& frame, const size_t hop)
{
  frame.assign(
    input.data() + hop,
    input.data() + (hop + frame.size()));
}

void STFT::inject(std::vector<float>& output, const std::vector<float>& frame, const size_t hop)
{
  for (size_t i = 0; i < frame.size(); ++i)
  {
    if ((hop + i) >= output.size())
    {
      break;
    }

    output[hop + i] += frame[i];
  }
}

void STFT::weight(std::vector<float>& frame, const std::vector<float>& window, const float scale)
{
  assert(frame.size() == window.size());

  for (size_t i = 0; i < frame.size(); ++i)
  {
    frame[i] *= window[i] * scale;
  }
}

void STFT::fft(const std::vector<float>& frame, std::vector<std::complex<float>>& dft, const float scale)
{
  assert(frame.size() / 2 + 1 == dft.size());

  pocketfft::r2c(
    { frame.size() },
    { sizeof(float) },
    { sizeof(std::complex<float>) },
    0,
    true,
    frame.data(),
    dft.data(),
    scale);
}

void STFT::ifft(const std::vector<std::complex<float>>& dft, std::vector<float>& frame, const float scale)
{
  assert(dft.size() == frame.size() / 2 + 1);

  pocketfft::c2r(
    { frame.size() },
    { sizeof(std::complex<float>) },
    { sizeof(float) },
    0,
    false,
    dft.data(),
    frame.data(),
    scale);
}
