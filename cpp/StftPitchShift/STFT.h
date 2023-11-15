#pragma once

#include <algorithm>
#include <cmath>
#include <complex>
#include <functional>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <tuple>
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
      STFT(fft, std::make_tuple(framesize, framesize), hopsize, chronometry)
    {
    }

    STFT(const std::shared_ptr<FFT> fft, const std::tuple<size_t, size_t> framesize, const size_t hopsize, const bool chronometry = false) :
      fft(fft),
      framesize(framesize),
      hopsize(hopsize),
      chronometry(chronometry)
    {
      const auto analysis_window_size = std::get<0>(framesize);
      const auto synthesis_window_size = std::get<1>(framesize);

      if (analysis_window_size < synthesis_window_size)
      {
        throw std::runtime_error(
          "Analysis window (" + std::to_string(analysis_window_size) + ") must be greater" +
          " or equal to synthesis window (" + std::to_string(synthesis_window_size) + ")!");
      }

      if (analysis_window_size == synthesis_window_size)
      {
        windows.analysis = symmetric_window(analysis_window_size);
        windows.synthesis = windows.analysis;
      }
      else
      {
        windows.analysis  = asymmetric_analysis_window(analysis_window_size, synthesis_window_size);
        windows.synthesis = asymmetric_synthesis_window(analysis_window_size, synthesis_window_size);
      }

      const T unitygain = hopsize / std::inner_product(
        windows.analysis.begin(), windows.analysis.end(), windows.synthesis.begin(), T(0));

      std::transform(windows.synthesis.begin(), windows.synthesis.end(), windows.synthesis.begin(),
        [unitygain](T value) { return value * unitygain; });
    }

    void operator()(const std::vector<T>& input, std::vector<T>& output, const std::function<void(std::vector<std::complex<T>>& dft)> callback) const
    {
      (*this)(input.size(), input.data(), output.data(), callback);
    }

    void operator()(const size_t size, const T* input, T* const output, const std::function<void(std::vector<std::complex<T>>& dft)> callback) const
    {
      const auto analysis_window_size = std::get<0>(framesize);
      const auto synthesis_window_size = std::get<1>(framesize);

      // TODO preemptively clear output memory #30
      std::fill(output, output + size, T(0));

      std::vector<T> frame(analysis_window_size);
      std::vector<std::complex<T>> dft(analysis_window_size / 2 + 1);

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
        for (size_t hop = 0; (hop + synthesis_window_size) < size; hop += hopsize)
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
        for (size_t hop = 0; (hop + synthesis_window_size) < size; hop += hopsize)
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
    const std::tuple<size_t, size_t> framesize;
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

      if (false) // optionally zero dc and nyquist
      {
        dft[0] = 0;
        dft[dft.size() - 1] = 0;
      }
    }

    inline void transform(std::vector<std::complex<T>>& dft, std::vector<T>& frame) const
    {
      if (true) // optionally zero dc and nyquist
      {
        dft[0] = 0;
        dft[dft.size() - 1] = 0;
      }

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

    static std::vector<T> symmetric_window(const size_t symmetric_window_size)
    {
      const auto n = symmetric_window_size;

      std::vector<T> window(n);

      const T pi = T(2) * std::acos(T(-1)) / n;

      for (size_t i = 0; i < n; ++i)
      {
        window[i] = T(0.5) - T(0.5) * std::cos(pi * i);
      }

      return window;
    }

    static std::vector<T> asymmetric_analysis_window(const size_t analysis_window_size, const size_t synthesis_window_size)
    {
      const auto n = analysis_window_size;
      const auto m = synthesis_window_size / 2;

      const auto left = symmetric_window(2 * n - 2 * m);
      const auto right = symmetric_window(2 * m);

      std::vector<T> window(n);

      for (size_t i = 0; i < n - m; ++i)
      {
        window[i] = left[i];
      }

      for (size_t i = 0; i < m; ++i)
      {
        window[i + n - m] = right[i + m];
      }

      return window;
    }

    static std::vector<T> asymmetric_synthesis_window(const size_t analysis_window_size, const size_t synthesis_window_size)
    {
      const auto n = analysis_window_size;
      const auto m = synthesis_window_size / 2;

      const auto left = symmetric_window(2 * n - 2 * m);
      const auto right = symmetric_window(2 * m);

      std::vector<T> window(n);

      for (size_t i = 0; i < m; ++i)
      {
        window[i + n - m - m] = (right[i] * right[i]) / left[i + n - m - m];
      }

      for (size_t i = 0; i < m; ++i)
      {
        window[i + n - m] = right[i + m];
      }

      return window;
    }

  };
}
