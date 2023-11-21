#pragma once

#include <chrono>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

namespace stftpitchshift
{
  template<class> struct WellKnownTimerDuration : std::false_type {};
  template<> struct WellKnownTimerDuration<std::chrono::seconds> : std::true_type {};
  template<> struct WellKnownTimerDuration<std::chrono::milliseconds> : std::true_type {};
  template<> struct WellKnownTimerDuration<std::chrono::microseconds> : std::true_type {};
  template<> struct WellKnownTimerDuration<std::chrono::nanoseconds> : std::true_type {};

  template<class T>
  class Timer
  {

  public:

    Timer(const size_t capacity = 100000)
    {
      static_assert(WellKnownTimerDuration<T>::value, "s,ms,us,ns");

      data.reserve(capacity);
    }

    Timer(const Timer& other)
    {
      data.reserve(other.data.capacity());
      data.assign(other.data.begin(), other.data.end());
    }

    size_t capacity() const
    {
      return data.capacity();
    }

    size_t size() const
    {
      return data.size();
    }

    void cls()
    {
      data.clear();
    }

    double lap() const
    {
      if (data.empty())
      {
        return 0;
      }

      return data.front();
    }

    void tic()
    {
      timestamp = std::chrono::steady_clock::now();

      if (data.empty())
      {
        data.push_back(0);
      }
    }

    void toc()
    {
      if (data.empty())
      {
        tic();
      }

      const std::chrono::steady_clock::duration duration = std::chrono::steady_clock::now() - timestamp;
      const double value = std::chrono::duration_cast<T>(duration * 1e+3).count() * 1e-3;

      data.front() += value;

      data.push_back(value);
    }

    std::string str()
    {
      const size_t offset = 1;

      if (data.size() <= offset)
      {
        return "";
      }

      const std::map<intmax_t, std::string> units =
      {
        { 1000000000, "ns" },
        { 1000000, "us" },
        { 1000, "ms" },
        { 1, "s" }
      };

      const std::string unit = units.at(T::period::num * T::period::den);

      const size_t n = data.size() - offset;

      const double sum = std::accumulate(
        data.begin() + offset, data.end(), 0.0);

      const double sumsum = std::inner_product(
        data.begin() + offset, data.end(), data.begin() + offset, 0.0);

      const double mean = sum / n;
      const double stdev = std::sqrt(sumsum / n - mean * mean);

      std::ostringstream result;
      result.precision(3);
      result << mean << " ± " << stdev << " " << unit << " n=" << n;

      return result.str();
    }

  private:

    std::chrono::time_point<std::chrono::steady_clock> timestamp;
    std::vector<double> data;

  };
}
