#pragma once

#include <complex>
#include <vector>

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
   */
  StftPitchShift(
    const size_t framesize,
    const size_t hopsize,
    const float samplerate);

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

  const size_t framesize;
  const size_t hopsize;
  const float samplerate;

  struct
  {
    bool chronometry;
    bool dump;
    std::string filename;
    size_t fileindex;
  }
  debug;

  static void dump(const std::string& filename, const size_t fileindex, const std::vector<float>& data);
  static void dump(const std::string& filename, const size_t fileindex, const std::vector<std::complex<float>>& data);

};
