#include <StftPitchShift/StftPitchShift.h>

#include <StftPitchShift/FFT.h>
#include <StftPitchShift/RFFT.h>

#include <StftPitchShift/SDFT.h>
#include <StftPitchShift/STFT.h>

#include <StftPitchShift/StftPitchShiftCore.h>

using namespace stftpitchshift;

StftPitchShift::StftPitchShift(
  const size_t framesize,
  const size_t hopsize,
  const double samplerate,
  const bool normalization,
  const bool chronometry) :
  fft(std::make_shared<RFFT>()),
  framesize(framesize),
  hopsize(hopsize),
  samplerate(samplerate),
  normalization(normalization),
  chronometry(chronometry)
{
}

StftPitchShift::StftPitchShift(
  const std::shared_ptr<FFT> fft,
  const size_t framesize,
  const size_t hopsize,
  const double samplerate,
  const bool normalization,
  const bool chronometry) :
  fft(fft),
  framesize(framesize),
  hopsize(hopsize),
  samplerate(samplerate),
  normalization(normalization),
  chronometry(chronometry)
{
}

void StftPitchShift::shiftpitch(
  const std::vector<float>& input,
  std::vector<float>& output,
  const double factor,
  const double quefrency,
  const double distortion)
{
  const std::vector<double> factors = { factor };

  shiftpitch(
    input.size(),
    input.data(),
    output.data(),
    factors,
    quefrency,
    distortion);
}

void StftPitchShift::shiftpitch(
  const std::vector<double>& input,
  std::vector<double>& output,
  const double factor,
  const double quefrency,
  const double distortion)
{
  const std::vector<double> factors = { factor };

  shiftpitch(
    input.size(),
    input.data(),
    output.data(),
    factors,
    quefrency,
    distortion);
}

void StftPitchShift::shiftpitch(
  const size_t size,
  const float* input,
  float* const output,
  const double factor,
  const double quefrency,
  const double distortion)
{
  const std::vector<double> factors = { factor };

  shiftpitch(
    size,
    input,
    output,
    factors,
    quefrency,
    distortion);
}

void StftPitchShift::shiftpitch(
  const size_t size,
  const double* input,
  double* const output,
  const double factor,
  const double quefrency,
  const double distortion)
{
  const std::vector<double> factors = { factor };

  shiftpitch(
    size,
    input,
    output,
    factors,
    quefrency,
    distortion);
}

void StftPitchShift::shiftpitch(
  const std::vector<float>& input,
  std::vector<float>& output,
  const std::vector<double>& factors,
  const double quefrency,
  const double distortion)
{
  shiftpitch(
    input.size(),
    input.data(),
    output.data(),
    factors,
    quefrency,
    distortion);
}

void StftPitchShift::shiftpitch(
  const std::vector<double>& input,
  std::vector<double>& output,
  const std::vector<double>& factors,
  const double quefrency,
  const double distortion)
{
  shiftpitch(
    input.size(),
    input.data(),
    output.data(),
    factors,
    quefrency,
    distortion);
}

void StftPitchShift::shiftpitch(
  const size_t size,
  const float* input,
  float* const output,
  const std::vector<double>& factors,
  const double quefrency,
  const double distortion)
{
  StftPitchShiftCore<float> core(fft, framesize, hopsize, samplerate);

  core.factors(factors);
  core.quefrency(quefrency);
  core.distortion(distortion);
  core.normalization(normalization);

  if (hopsize == 1)
  {
    SDFT<float> sdft(framesize, /* latency */ 1, chronometry);

    sdft(size, input, output, [&](std::vector<std::complex<float>>& dft)
    {
      core.shiftpitch(dft);
    });
  }
  else
  {
    STFT<float> stft(fft, framesize, hopsize, chronometry);

    stft(size, input, output, [&](std::vector<std::complex<float>>& dft)
    {
      core.shiftpitch(dft);
    });
  }
}

void StftPitchShift::shiftpitch(
  const size_t size,
  const double* input,
  double* const output,
  const std::vector<double>& factors,
  const double quefrency,
  const double distortion)
{
  StftPitchShiftCore<double> core(fft, framesize, hopsize, samplerate);

  core.factors(factors);
  core.quefrency(quefrency);
  core.distortion(distortion);
  core.normalization(normalization);

  if (hopsize == 1)
  {
    SDFT<double> sdft(framesize, /* latency */ 1, chronometry);

    sdft(size, input, output, [&](std::vector<std::complex<double>>& dft)
    {
      core.shiftpitch(dft);
    });
  }
  else
  {
    STFT<double> stft(fft, framesize, hopsize, chronometry);

    stft(size, input, output, [&](std::vector<std::complex<double>>& dft)
    {
      core.shiftpitch(dft);
    });
  }
}
