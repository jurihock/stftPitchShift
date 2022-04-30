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

  public:

    Timer(const size_t capacity = 1000000)
    {
      data.reserve(capacity);
    }

    void cls()
    {
      data.clear();
    }

    void tic()
    {
      timestamp = std::chrono::high_resolution_clock::now();
    }

    void toc()
    {
      const std::chrono::high_resolution_clock::duration duration =
        std::chrono::high_resolution_clock::now() - timestamp;

      data.push_back(std::chrono::duration_cast<T>(duration * 1e+3).count() * 1e-3);
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
      result << mean << " ± " << stdev << " " << unit;

      return result.str();
    }

  private:

    std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;
    std::vector<double> data;

  };
}
