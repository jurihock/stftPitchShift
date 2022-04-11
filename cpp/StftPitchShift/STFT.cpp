#include <STFT.h>

#undef TIMERS

#ifdef TIMERS
#include <Timer.h>
#endif

#include <pocketfft/pocketfft_hdronly.h>

STFT::STFT(const size_t framesize, const size_t hopsize) :
  framesize(framesize),
  hopsize(hopsize)
{
  std::vector<float> window(framesize);

  std::iota(window.begin(), window.end(), 0.0f);

  std::transform(window.begin(), window.end(), window.begin(),
    [&](float value) { return 0.5f - 0.5f * std::cos(PI2 * value / window.size()); });

  windows.analysis = window;
  windows.synthesis = window;

  const float unitygain = hopsize / std::inner_product(
    window.begin(), window.end(), window.begin(), 0.0f);

  std::transform(windows.synthesis.begin(), windows.synthesis.end(), windows.synthesis.begin(),
    [&](float value) { return value * unitygain; });
}

void STFT::operator()(const std::vector<float>& input, std::vector<float>& output, const std::function<void(std::vector<std::complex<float>>& dft)> callback) const
{
  (*this)(input.size(), input.data(), output.data(), callback);
}

void STFT::operator()(const size_t size, const float* input, float* const output, const std::function<void(std::vector<std::complex<float>>& dft)> callback) const
{
  #ifdef TIMERS
  struct
  {
    Timer<std::chrono::microseconds> analysis;
    Timer<std::chrono::microseconds> synthesis;
    Timer<std::chrono::microseconds> callback;
    Timer<std::chrono::milliseconds> loop;
  }
  timers;
  #endif

  std::vector<float> frame(framesize);
  std::vector<std::complex<float>> dft(framesize / 2 + 1);

  #ifdef TIMERS
  timers.loop.tic();
  #endif

  for (size_t hop = 0; (hop + framesize) < size; hop += hopsize)
  {
    #ifdef TIMERS
    timers.analysis.tic();
    #endif

    reject(hop, input, frame, windows.analysis);
    fft(frame, dft);

    #ifdef TIMERS
    timers.analysis.toc();
    #endif

    #ifdef TIMERS
    timers.callback.tic();
    #endif

    callback(dft);

    #ifdef TIMERS
    timers.callback.toc();
    #endif

    #ifdef TIMERS
    timers.synthesis.tic();
    #endif

    ifft(dft, frame);
    inject(hop, output, frame, windows.synthesis);

    #ifdef TIMERS
    timers.synthesis.toc();
    #endif
  }

  #ifdef TIMERS
  timers.loop.toc();
  #endif

  #ifdef TIMERS
  std::cout << "analysis  " << timers.analysis.str()  << std::endl;
  std::cout << "synthesis " << timers.synthesis.str() << std::endl;
  std::cout << "callback  " << timers.callback.str()  << std::endl;
  std::cout << "loop      " << timers.loop.str()      << std::endl;
  #endif
}

void STFT::reject(const size_t hop, const float* input, std::vector<float>& frame, const std::vector<float>& window)
{
  for (size_t i = 0; i < frame.size(); ++i)
  {
    frame[i] = input[hop + i] * window[i];
  }
}

void STFT::inject(const size_t hop, float* const output, const std::vector<float>& frame, const std::vector<float>& window)
{
  for (size_t i = 0; i < frame.size(); ++i)
  {
    output[hop + i] += frame[i] * window[i];
  }
}

void STFT::fft(const std::vector<float>& frame, std::vector<std::complex<float>>& dft)
{
  pocketfft::r2c(
    { frame.size() },
    { sizeof(float) },
    { sizeof(std::complex<float>) },
    0,
    true,
    frame.data(),
    dft.data(),
    1.0f / frame.size());
}

void STFT::ifft(const std::vector<std::complex<float>>& dft, std::vector<float>& frame)
{
  pocketfft::c2r(
    { frame.size() },
    { sizeof(std::complex<float>) },
    { sizeof(float) },
    0,
    false,
    dft.data(),
    frame.data(),
    1.0f);
}
