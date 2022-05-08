#pragma once

#include <StftPitchShift/FFT.h>

namespace stftpitchshift
{
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
     * @param normalization Optionally enable spectral rms normalization.
     * @param chronometry Optionally enable runtime measurements.
     */
    StftPitchShift(
      const size_t framesize,
      const size_t hopsize,
      const double samplerate,
      const bool normalization = false,
      const bool chronometry = false);

    /**
     * @param fft The custom FFT calculation instance.
     * @param framesize The STFT frame size in samples.
     * @param hopsize The STFT hop size in samples.
     * @param samplerate The sample rate of the signal in hertz.
     * @param normalization Optionally enable spectral rms normalization.
     * @param chronometry Optionally enable runtime measurements.
     */
    StftPitchShift(
      const std::shared_ptr<FFT> fft,
      const size_t framesize,
      const size_t hopsize,
      const double samplerate,
      const bool normalization = false,
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
      const double factor = 1,
      const double quefrency = 0);

    /**
     * @param input The input signal.
     * @param output The output signal of the equal size.
     * @param factor The fractional pitch shifting factor.
     * @param quefrency The optional formant lifter quefrency in seconds.
     */
    void shiftpitch(
      const std::vector<double>& input,
      std::vector<double>& output,
      const double factor = 1,
      const double quefrency = 0);

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
      const double factor = 1,
      const double quefrency = 0);

    /**
     * @param size The total number of signal values, not bytes.
     * @param input The input signal.
     * @param output The output signal of the equal size.
     * @param factor The fractional pitch shifting factor.
     * @param quefrency The optional formant lifter quefrency in seconds.
     */
    void shiftpitch(
      const size_t size,
      const double* input,
      double* const output,
      const double factor = 1,
      const double quefrency = 0);

    /**
     * @param input The input signal.
     * @param output The output signal of the equal size.
     * @param factors The fractional pitch shifting factors.
     * @param quefrency The optional formant lifter quefrency in seconds.
     */
    void shiftpitch(
      const std::vector<float>& input,
      std::vector<float>& output,
      const std::vector<double>& factors,
      const double quefrency = 0);

    /**
     * @param input The input signal.
     * @param output The output signal of the equal size.
     * @param factors The fractional pitch shifting factors.
     * @param quefrency The optional formant lifter quefrency in seconds.
     */
    void shiftpitch(
      const std::vector<double>& input,
      std::vector<double>& output,
      const std::vector<double>& factors,
      const double quefrency = 0);

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
      const std::vector<double>& factors,
      const double quefrency = 0);

    /**
     * @param size The total number of signal values, not bytes.
     * @param input The input signal.
     * @param output The output signal of the equal size.
     * @param factors The fractional pitch shifting factors.
     * @param quefrency The optional formant lifter quefrency in seconds.
     */
    void shiftpitch(
      const size_t size,
      const double* input,
      double* const output,
      const std::vector<double>& factors,
      const double quefrency = 0);

  private:

    const std::shared_ptr<FFT> fft;
    const size_t framesize;
    const size_t hopsize;
    const double samplerate;
    const bool normalization;
    const bool chronometry;

  };
}
