#pragma once

#include <StftPitchShift/FFT.h>

/**
 * Short-time Fourier transform (STFT) based pitch shifting.
 */
class StftPitchShift
{

public:

  /**
   * @param framesize The STFT frame size in samples.
   * @param hopsize The STFT hop size in samples.
   * @param samplerate The sample rate of the signal in hertz.
   * @param chronometry Optionally enable runtime measurements.
   */
  StftPitchShift(
    const size_t framesize,
    const size_t hopsize,
    const float samplerate,
    const bool chronometry = false);

  /**
   * @param fft The custom FFT calculation instance.
   * @param framesize The STFT frame size in samples.
   * @param hopsize The STFT hop size in samples.
   * @param samplerate The sample rate of the signal in hertz.
   * @param chronometry Optionally enable runtime measurements.
   */
  StftPitchShift(
    const std::shared_ptr<FFT> fft,
    const size_t framesize,
    const size_t hopsize,
    const float samplerate,
    const bool chronometry = false);

  /**
   * @param input The input signal.
   * @param output The output signal of the equal size.
   * @param factor The fractional pitch shifting factor.
   * @param quefrency The optional formant lifter quefrency in seconds.
   */
  void shiftpitch(
    const std::vector<float>& input,
    std::vector<float>& output,
    const float factor = 1,
    const float quefrency = 0);

  /**
   * @param size The total number of signal values, not bytes.
   * @param input The input signal.
   * @param output The output signal of the equal size.
   * @param factor The fractional pitch shifting factor.
   * @param quefrency The optional formant lifter quefrency in seconds.
   */
  void shiftpitch(
    const size_t size,
    const float* input,
    float* const output,
    const float factor = 1,
    const float quefrency = 0);

  /**
   * @param input The input signal.
   * @param output The output signal of the equal size.
   * @param factors The fractional pitch shifting factors.
   * @param quefrency The optional formant lifter quefrency in seconds.
   */
  void shiftpitch(
    const std::vector<float>& input,
    std::vector<float>& output,
    const std::vector<float>& factors,
    const float quefrency = 0);

  /**
   * @param size The total number of signal values, not bytes.
   * @param input The input signal.
   * @param output The output signal of the equal size.
   * @param factors The fractional pitch shifting factors.
   * @param quefrency The optional formant lifter quefrency in seconds.
   */
  void shiftpitch(
    const size_t size,
    const float* input,
    float* const output,
    const std::vector<float>& factors,
    const float quefrency = 0);

private:

  const std::shared_ptr<FFT> fft;
  const size_t framesize;
  const size_t hopsize;
  const float samplerate;
  const bool chronometry;

};
