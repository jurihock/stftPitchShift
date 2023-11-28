#include <StftPitchShift/StftPitchShift.h>

#include <StftPitchShift/FFT.h>
#include <StftPitchShift/RFFT.h>
#include <StftPitchShift/STFT.h>

#include <StftPitchShift/StftPitchShiftCore.h>

using namespace stftpitchshift;

StftPitchShift::StftPitchShift(
  const size_t framesize,
  const size_t hopsize,
  const double samplerate,
  const bool normalization,
  const bool chronometry) :
  StftPitchShift(
    std::make_shared<RFFT>(),
    std::make_tuple(framesize, framesize),
    hopsize,
    samplerate,
    normalization,
    chronometry)
{
}

StftPitchShift::StftPitchShift(
  const std::tuple<size_t, size_t> framesize,
  const size_t hopsize,
  const double samplerate,
  const bool normalization,
  const bool chronometry) :
  StftPitchShift(
    std::make_shared<RFFT>(),
    framesize,
    hopsize,
    samplerate,
    normalization,
    chronometry)
{
}

StftPitchShift::StftPitchShift(
  const std::shared_ptr<FFT> fft,
  const size_t framesize,
  const size_t hopsize,
  const double samplerate,
  const bool normalization,
  const bool chronometry) :
  StftPitchShift(
    fft,
    std::make_tuple(framesize, framesize),
    hopsize,
    samplerate,
    normalization,
    chronometry)
{
}

StftPitchShift::StftPitchShift(
  const std::shared_ptr<FFT> fft,
  const std::tuple<size_t, size_t> framesize,
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
  const std::span<const float> input,
  const std::span<float> output,
  const double factor,
  const double quefrency,
  const double distortion)
{
  const std::vector<double> factors = { factor };

  shiftpitch(
    input,
    output,
    factors,
    quefrency,
    distortion);
}

void StftPitchShift::shiftpitch(
  const std::span<const double> input,
  const std::span<double> output,
  const double factor,
  const double quefrency,
  const double distortion)
{
  const std::vector<double> factors = { factor };

  shiftpitch(
    input,
    output,
    factors,
    quefrency,
    distortion);
}

void StftPitchShift::shiftpitch(
  const std::span<const float> input,
  const std::span<float> output,
  const std::vector<double>& factors,
  const double quefrency,
  const double distortion)
{
  // preemptively clear output #30
  std::fill(output.begin(), output.end(), float(0));

  StftPitchShiftCore<float> core(fft, framesize, hopsize, samplerate);

  core.factors(factors);
  core.quefrency(quefrency);
  core.distortion(distortion);
  core.normalization(normalization);

  STFT<float> stft(fft, framesize, hopsize, chronometry);

  stft(input, output, [&](std::span<std::complex<float>> dft)
  {
    core.shiftpitch(dft);
  });
}

void StftPitchShift::shiftpitch(
  const std::span<const double> input,
  const std::span<double> output,
  const std::vector<double>& factors,
  const double quefrency,
  const double distortion)
{
  // preemptively clear output #30
  std::fill(output.begin(), output.end(), double(0));

  StftPitchShiftCore<double> core(fft, framesize, hopsize, samplerate);

  core.factors(factors);
  core.quefrency(quefrency);
  core.distortion(distortion);
  core.normalization(normalization);

  STFT<double> stft(fft, framesize, hopsize, chronometry);

  stft(input, output, [&](std::span<std::complex<double>> dft)
  {
    core.shiftpitch(dft);
  });
}
