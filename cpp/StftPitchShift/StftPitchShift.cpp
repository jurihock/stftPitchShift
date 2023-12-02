#include <StftPitchShift/StftPitchShift.h>

#include <StftPitchShift/FFT.h>
#include <StftPitchShift/RFFT.h>
#include <StftPitchShift/STFT.h>

#include <StftPitchShift/StftPitchShiftCore.h>

using namespace stftpitchshift;

StftPitchShift::StftPitchShift(
  const double samplerate,
  const size_t framesize,
  const size_t hopsize,
  const size_t padsize,
  const bool normalization,
  const bool chronometry) :
  StftPitchShift(
    std::make_shared<RFFT>(),
    samplerate,
    std::make_tuple(framesize, framesize),
    hopsize,
    padsize,
    normalization,
    chronometry)
{
}

StftPitchShift::StftPitchShift(
  const double samplerate,
  const std::tuple<size_t, size_t> framesize,
  const size_t hopsize,
  const size_t padsize,
  const bool normalization,
  const bool chronometry) :
  StftPitchShift(
    std::make_shared<RFFT>(),
    samplerate,
    framesize,
    hopsize,
    padsize,
    normalization,
    chronometry)
{
}

StftPitchShift::StftPitchShift(
  const std::shared_ptr<FFT> fft,
  const double samplerate,
  const size_t framesize,
  const size_t hopsize,
  const size_t padsize,
  const bool normalization,
  const bool chronometry) :
  StftPitchShift(
    fft,
    samplerate,
    std::make_tuple(framesize, framesize),
    hopsize,
    padsize,
    normalization,
    chronometry)
{
}

StftPitchShift::StftPitchShift(
  const std::shared_ptr<FFT> fft,
  const double samplerate,
  const std::tuple<size_t, size_t> framesize,
  const size_t hopsize,
  const size_t padsize,
  const bool normalization,
  const bool chronometry) :
  fft(fft),
  samplerate(samplerate),
  framesize(framesize),
  hopsize(hopsize),
  padsize(padsize),
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

  StftPitchShiftCore<float> core(fft, samplerate, framesize, hopsize, padsize);

  core.factors(factors);
  core.quefrency(quefrency);
  core.distortion(distortion);
  core.normalization(normalization);

  STFT<float> stft(fft, framesize, hopsize, padsize, chronometry);

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

  StftPitchShiftCore<double> core(fft, samplerate, framesize, hopsize, padsize);

  core.factors(factors);
  core.quefrency(quefrency);
  core.distortion(distortion);
  core.normalization(normalization);

  STFT<double> stft(fft, framesize, hopsize, padsize, chronometry);

  stft(input, output, [&](std::span<std::complex<double>> dft)
  {
    core.shiftpitch(dft);
  });
}
