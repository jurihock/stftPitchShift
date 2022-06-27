#pragma once

#include <algorithm>
#include <cmath>
#include <complex>
#include <functional>
#include <numeric>
#include <vector>

#include <StftPitchShift/Timer.h>

namespace stftpitchshift
{
  template<class T>
  class SDFT
  {

  public:

    SDFT(const size_t framesize, const double latency, const bool chronometry = false) :
      framesize(framesize),
      dftsize(framesize / 2),
      latency(1),
      chronometry(chronometry)
    {
      analysis.roi = { 0, dftsize };
      synthesis.roi = { 0, dftsize };

      analysis.twiddles.resize(dftsize);
      synthesis.twiddles.resize(dftsize);

      analysis.cursor = 0;
      analysis.input.resize(dftsize * 2);
      analysis.accoutput.resize(dftsize);
      analysis.auxoutput.resize(dftsize + 2);
      analysis.fiddles.resize(dftsize, 1);

      const double pi = -2.0 * std::acos(-1.0) / (dftsize * 2);
      const double weight = 2.0 / (1.0 - std::cos(pi * dftsize * latency));

      for (size_t i = 0; i < dftsize; ++i)
      {
        analysis.twiddles[i] = std::polar(1.0, pi * i);
        synthesis.twiddles[i] = std::polar(weight, pi * i * dftsize * latency);
      }
    }

    void operator()(const std::vector<T>& input, std::vector<T>& output, const std::function<void(std::vector<std::complex<T>>& dft)> callback)
    {
      (*this)(input.size(), input.data(), output.data(), callback);
    }

    void operator()(const size_t size, const T* input, T* const output, const std::function<void(std::vector<std::complex<T>>& dft)> callback)
    {
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
        for (size_t i = 0; i < size; ++i)
        {
          timers.analysis.tic();
          sdft(input[i], dft);
          timers.analysis.toc();

          timers.callback.tic();
          callback(dft);
          timers.callback.toc();

          timers.synthesis.tic();
          output[i] = isdft(dft);
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
        for (size_t i = 0; i < size; ++i)
        {
          sdft(input[i], dft);
          callback(dft);
          output[i] = isdft(dft);
        }
      }
    }

  private:

    const size_t framesize;
    const size_t dftsize;
    const double latency;
    const bool chronometry;

    struct
    {
      std::pair<size_t, size_t> roi;
      std::vector<std::complex<double>> twiddles;

      size_t cursor;
      std::vector<T> input;
      std::vector<std::complex<double>> accoutput;
      std::vector<std::complex<double>> auxoutput;
      std::vector<std::complex<double>> fiddles;
    }
    analysis;

    struct
    {
      std::pair<size_t, size_t> roi;
      std::vector<std::complex<double>> twiddles;
    }
    synthesis;

    inline static std::complex<T> window(const std::complex<double>& left,
                                         const std::complex<double>& middle,
                                         const std::complex<double>& right,
                                         const double weight)
    {
      // the factor 1/4 is already included in the weight

      const std::complex<double> value =
        /* 0.25 */ ((middle + middle) - (left + right)) * weight;

      return std::complex<T>(
        static_cast<T>(value.real()),
        static_cast<T>(value.imag()));
    }

    inline static T exchange(T& oldvalue, const T newvalue)
    {
      const T oldcopy = oldvalue;

      oldvalue = newvalue;

      return oldcopy;
    }

    void sdft(const T sample, std::vector<std::complex<T>>& dft)
    {
      // actually the weight denominator needs to be dftsize*2 to get proper magnitude scaling,
      // but then requires a correction by factor 2 in synthesis and is therefore omitted

      const double weight = 0.25 / dftsize; // incl. factor 1/4 for windowing

      const double delta = sample - exchange(analysis.input[analysis.cursor], sample);

      for (size_t i = analysis.roi.first, j = i + 1; i < analysis.roi.second; ++i, ++j)
      {
        const std::complex<double> oldfiddle = analysis.fiddles[i];
        const std::complex<double> newfiddle = oldfiddle * analysis.twiddles[i];

        analysis.fiddles[i] = newfiddle;

        analysis.accoutput[i] += delta * oldfiddle;
        analysis.auxoutput[j] = analysis.accoutput[i] * std::conj(newfiddle);
      }

      // theoretically the DFT periodicity needs to be preserved for proper windowing,
      // but the both outer bins seem to be noisy for an unclear reason
      // and will be suppressed anyway after windowing

      // analysis.auxoutput[0] = analysis.auxoutput[dftsize];
      // analysis.auxoutput[dftsize + 1] = analysis.auxoutput[1];

      for (size_t i = analysis.roi.first, j = i + 1; i < analysis.roi.second; ++i, ++j)
      {
        dft[i] = window(analysis.auxoutput[j - 1],
                        analysis.auxoutput[j],
                        analysis.auxoutput[j + 1],
                        weight);
      }

      // finally suppress outer DFT bins as announced in the comment above

      dft[0] = dft[dftsize - 1] = 0;

      if (++analysis.cursor >= analysis.input.size())
      {
        analysis.cursor = 0;

        // TODO std::fill(analysis.fiddles.begin(), analysis.fiddles.end(), 1);
      }
    }

    T isdft(const std::vector<std::complex<T>>& dft)
    {
      double sample = 0;

      if (latency == 1)
      {
        for (size_t i = synthesis.roi.first; i < synthesis.roi.second; ++i)
        {
          const double value = dft[i].real();

          sample += value * (i % 2 ? -1 : +1);
        }
      }
      else
      {
        for (size_t i = synthesis.roi.first; i < synthesis.roi.second; ++i)
        {
          const std::complex<double> value = { dft[i].real(), dft[i].imag() };

          sample += (value * synthesis.twiddles[i]).real();
        }
      }

      return static_cast<T>(sample);
    }

  };
}
