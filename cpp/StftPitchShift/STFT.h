#pragma once

#include <algorithm>
#include <cmath>
#include <complex>
#include <functional>
#include <numeric>
#include <vector>

#include <StftPitchShift/FFT.h>
#include <StftPitchShift/Timer.h>

namespace stftpitchshift
{
  template<class T>
  class STFT
  {

  public:

    STFT(const std::shared_ptr<FFT> fft, const size_t framesize, const size_t hopsize, const bool chronometry = false) :
      fft(fft),
      framesize(framesize),
      hopsize(hopsize),
      chronometry(chronometry)
    {
      const T PI2 = T(2) * std::acos(T(-1));

      std::vector<T> window(framesize);

      std::iota(window.begin(), window.end(), T(0));

      std::transform(window.begin(), window.end(), window.begin(),
        [&](T value) { return T(0.5) - T(0.5) * std::cos(PI2 * value / window.size()); });

      windows.analysis = window;
      windows.synthesis = window;

      const T unitygain = hopsize / std::inner_product(
        window.begin(), window.end(), window.begin(), T(0));

      std::transform(windows.synthesis.begin(), windows.synthesis.end(), windows.synthesis.begin(),
        [&](T value) { return value * unitygain; });
    }

    void operator()(const std::vector<T>& input, std::vector<T>& output, const std::function<void(std::vector<std::complex<T>>& dft)> callback) const
    {
      (*this)(input.size(), input.data(), output.data(), callback);
    }

    void operator()(const size_t size, const T* input, T* const output, const std::function<void(std::vector<std::complex<T>>& dft)> callback) const
    {
      std::vector<T> frame(framesize);
      std::vector<std::complex<T>> dft(framesize / 2 + 1);

      if (chronometry)
      {
        struct
        {
          Timer<std::chrono::microseconds> analysis;
          Timer<std::chrono::microseconds> synthesis;
          Timer<std::chrono::microseconds> callback;
          Timer<std::chrono::milliseconds> loop;
        }
        timers;

        timers.loop.tic();
        for (size_t hop = 0; (hop + framesize) < size; hop += hopsize)
        {
          timers.analysis.tic();
          reject(hop, input, frame, windows.analysis);
          transform(frame, dft);
          timers.analysis.toc();

          timers.callback.tic();
          callback(dft);
          timers.callback.toc();

          timers.synthesis.tic();
          transform(dft, frame);
          inject(hop, output, frame, windows.synthesis);
          timers.synthesis.toc();
        }
        timers.loop.toc();

        std::cout << "analysis  " << timers.analysis.str()  << std::endl;
        std::cout << "synthesis " << timers.synthesis.str() << std::endl;
        std::cout << "callback  " << timers.callback.str()  << std::endl;
        std::cout << "loop      " << timers.loop.str()      << std::endl;
      }
      else
      {
        for (size_t hop = 0; (hop + framesize) < size; hop += hopsize)
        {
          reject(hop, input, frame, windows.analysis);
          transform(frame, dft);

          callback(dft);

          transform(dft, frame);
          inject(hop, output, frame, windows.synthesis);
        }
      }
    }

  private:

    const std::shared_ptr<FFT> fft;
    const size_t framesize;
    const size_t hopsize;
    const bool chronometry;

    struct
    {
      std::vector<T> analysis;
      std::vector<T> synthesis;
    }
    windows;

    inline void transform(std::vector<T>& frame, std::vector<std::complex<T>>& dft) const
    {
      fft->fft(frame, dft);

      // keep dc and nyquist as is
      // dft[0] = dft[dft.size() - 1] = 0;
    }

    inline void transform(std::vector<std::complex<T>>& dft, std::vector<T>& frame) const
    {
      // zero dc and nyquist
      dft[0] = dft[dft.size() - 1] = 0;

      fft->ifft(dft, frame);
    }

    static void reject(const size_t hop, const T* input, std::vector<T>& frame, const std::vector<T>& window)
    {
      for (size_t i = 0; i < frame.size(); ++i)
      {
        frame[i] = input[hop + i] * window[i];
      }
    }

    static void inject(const size_t hop, T* const output, const std::vector<T>& frame, const std::vector<T>& window)
    {
      for (size_t i = 0; i < frame.size(); ++i)
      {
        output[hop + i] += frame[i] * window[i];
      }
    }

  };
}
