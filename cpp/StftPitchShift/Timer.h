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
  template<class T>
  class Timer
  {

  private:

    template<class> struct is_valid_duration : std::false_type {};
    template<> struct is_valid_duration<std::chrono::seconds> : std::true_type {};
    template<> struct is_valid_duration<std::chrono::milliseconds> : std::true_type {};
    template<> struct is_valid_duration<std::chrono::microseconds> : std::true_type {};
    template<> struct is_valid_duration<std::chrono::nanoseconds> : std::true_type {};

  public:

    Timer(const size_t capacity = 100000)
    {
      static_assert(is_valid_duration<T>::value, "s,ms,us,ns");

      data.reserve(capacity);
    }

    Timer(const Timer& other)
    {
      data.reserve(other.data.capacity());
      data.assign(other.data.begin(), other.data.end());
    }

    void cls()
    {
      data.clear();
    }

    void tic()
    {
      timestamp = std::chrono::steady_clock::now();
    }

    void toc()
    {
      const std::chrono::steady_clock::duration duration = std::chrono::steady_clock::now() - timestamp;
      const double value = std::chrono::duration_cast<T>(duration * 1e+3).count() * 1e-3;

      data.push_back(value);
    }

    std::string str()
    {
      const std::map<intmax_t, std::string> units =
      {
        { 1000000000, "ns" },
        { 1000000, "us" },
        { 1000, "ms" },
        { 1, "s" }
      };

      const std::string unit = units.at(T::period::num * T::period::den);

      const double sum = std::accumulate(data.begin(), data.end(), 0.0);
      const double sumsum = std::inner_product(data.begin(), data.end(), data.begin(), 0.0);

      const double mean = sum / data.size();
      const double stdev = std::sqrt(sumsum / data.size() - mean * mean);

      std::ostringstream result;
      result.precision(3);
      result << mean << " ± " << stdev << " " << unit << " n=" << data.size();

      return result.str();
    }

  private:

    std::chrono::time_point<std::chrono::steady_clock> timestamp;
    std::vector<double> data;

  };
}
