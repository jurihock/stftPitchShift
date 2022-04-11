#include <StftPitchShift.h>

#include <Cepstrum.h>
#include <Pitcher.h>
#include <STFT.h>
#include <Vocoder.h>

StftPitchShift::StftPitchShift(
  const size_t framesize,
  const size_t hopsize,
  const float samplerate) :
  framesize(framesize),
  hopsize(hopsize),
  samplerate(samplerate)
{
}

void StftPitchShift::shiftpitch(
  const std::vector<float>& input,
  std::vector<float>& output,
  const float factor,
  const float quefrency)
{
  const std::vector<float> factors = { factor };

  shiftpitch(
    input.size(),
    input.data(),
    output.data(),
    factors,
    quefrency);
}

void StftPitchShift::shiftpitch(
  const size_t size,
  const float* input,
  float* const output,
  const float factor,
  const float quefrency)
{
  const std::vector<float> factors = { factor };

  shiftpitch(
    size,
    input,
    output,
    factors,
    quefrency);
}

void StftPitchShift::shiftpitch(
  const std::vector<float>& input,
  std::vector<float>& output,
  const std::vector<float>& factors,
  const float quefrency)
{
  shiftpitch(
    input.size(),
    input.data(),
    output.data(),
    factors,
    quefrency);
}

void StftPitchShift::shiftpitch(
  const size_t size,
  const float* input,
  float* const output,
  const std::vector<float>& factors,
  const float quefrency)
{
  STFT stft(framesize, hopsize);
  Vocoder vocoder(framesize, hopsize, samplerate);
  Pitcher pitcher(factors);
  Cepstrum cepstrum(quefrency, samplerate);

  if (quefrency)
  {
    std::vector<float> envelope;

    stft(size, input, output, [&](std::vector<std::complex<float>>& frame)
    {
      vocoder.encode(frame);

      cepstrum.lifter(frame, envelope);

      for (size_t i = 0; i < frame.size(); ++i)
      {
        frame[i].real(frame[i].real() / envelope[i]);
      }

      pitcher.shiftpitch(frame);

      for (size_t i = 0; i < frame.size(); ++i)
      {
        frame[i].real(frame[i].real() * envelope[i]);
      }

      vocoder.decode(frame);
    });
  }
  else
  {
    stft(size, input, output, [&](std::vector<std::complex<float>>& frame)
    {
      vocoder.encode(frame);
      pitcher.shiftpitch(frame);
      vocoder.decode(frame);
    });
  }
}
