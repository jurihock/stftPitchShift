#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <span>
#include <tuple>
#include <vector>

#include <StftPitchShift/STFT.h>
#include <StftPitchShift/StftPitchShiftCore.h>

using namespace stftpitchshift;

// basic parameters
// - samplerate as required
// - overlap at least 4
const double samplerate = 44100;
const size_t overlap = 4;

// analysis and synthesis window size
// power of two each of them
// - for best quality: analysis = synthesis
// - for lower latency: analysis > synthesis
// - synthesis window size corresponds to the audio frame size and vice versa
const std::tuple<size_t, size_t> framesize = { 1024, 1024 };

// hop size refers to the synthesis window size
const size_t hopsize = std::get<1>(framesize) / overlap;

// delay buffers for input and output frames
struct { std::vector<double> input, output; } buffer;

// modules required for pitch shifting
std::shared_ptr<STFT<double>> stft;
std::shared_ptr<StftPitchShiftCore<double>> core;

// example of an IO callback
void audio_interface_callback(std::span<float> input, std::span<float> output);

int main()
{
  // prepare for audio stream processing

  const size_t total_buffer_size =
    std::get<0>(framesize) +
    std::get<1>(framesize);

  buffer.input.resize(total_buffer_size);
  buffer.output.resize(total_buffer_size);

  stft = std::make_shared<STFT<double>>(framesize, hopsize);
  core = std::make_shared<StftPitchShiftCore<double>>(framesize, hopsize, samplerate);

  // set pitch shifting parameters as required

  core->factors({ 1 });
  core->quefrency(0 * 1e-3);
  core->distortion(1);
  core->normalization(false);

  // simulate continuous audio stream processing

  const size_t n = static_cast<size_t>(samplerate);
  const size_t m = std::get<1>(framesize);

  std::vector<float> x(n), y(n);

  for (size_t i = 0; (i + m) < n; i += m)
  {
    std::cout << "processing audio frame " << (i / m + 1) << " of " << (n / m) << std::endl;

    std::span<float> input = std::span(x.data(), m);
    std::span<float> output = std::span(y.data(), m);

    audio_interface_callback(input, output);
  }

  return 0;
}

void audio_interface_callback(std::span<float> input, std::span<float> output)
{
  const auto analysis_window_size = std::get<0>(framesize);
  const auto synthesis_window_size = std::get<1>(framesize);

  // this is the expected input and output frame size
  assert(input.size()  == synthesis_window_size);
  assert(output.size() == synthesis_window_size);

  // shift input buffer
  std::copy(
    buffer.input.begin() + synthesis_window_size,
    buffer.input.end(),
    buffer.input.begin());

  // copy new input samples
  std::copy(
    input.begin(),
    input.end(),
    buffer.input.begin() + analysis_window_size);

  // apply pitch shifting within the built-in STFT routine
  (*stft)(buffer.input, buffer.output, [&](std::span<std::complex<double>> dft)
  {
    core->shiftpitch(dft);
  });

  // copy new output samples back
  std::copy(
    buffer.output.begin() - synthesis_window_size + analysis_window_size,
    buffer.output.end() - synthesis_window_size,
    output.begin());

  // shift output buffer
  std::copy(
    buffer.output.begin() + synthesis_window_size,
    buffer.output.end(),
    buffer.output.begin());

  // prepare for the next callback
  std::fill(
    buffer.output.begin() + analysis_window_size,
    buffer.output.end(),
    0);
}
