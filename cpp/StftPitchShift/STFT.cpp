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

  const float scalers[4] =
  {
    1.0f,             // analysis window
    1.0f / framesize, // forward fft
    1.0f,             // backward fft
    1.0f * unitygain  // synthesis window
  };

  #ifdef TIMERS
  timers.loop.tic();
  #endif

  for (size_t hop = 0; (hop + framesize) < size; hop += hopsize)
  {
    #ifdef TIMERS
    timers.analysis.tic();
    #endif

    reject(/*size,*/ input, frame, hop);
    weight(frame, window, scalers[0]);
    fft(frame, dft, scalers[1]);

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

    ifft(dft, frame, scalers[2]);
    weight(frame, window, scalers[3]);
    inject(size, output, frame, hop);

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

void STFT::reject(/*const size_t size,*/ const float* input, std::vector<float>& frame, const size_t hop)
{
  frame.assign(
    input + hop,
    input + hop + frame.size());
}

void STFT::inject(const size_t size, float* const output, const std::vector<float>& frame, const size_t hop)
{
  for (size_t i = 0; i < frame.size(); ++i)
  {
    if ((hop + i) >= size)
    {
      break;
    }

    output[hop + i] += frame[i];
  }
}

void STFT::weight(std::vector<float>& frame, const std::vector<float>& window, const float scale)
{
  for (size_t i = 0; i < frame.size(); ++i)
  {
    frame[i] *= window[i] * scale;
  }
}

void STFT::fft(const std::vector<float>& frame, std::vector<std::complex<float>>& dft, const float scale)
{
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
