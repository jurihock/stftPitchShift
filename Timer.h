#pragma once

#include <chrono>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

template<class T>
class Timer
{

public:

  Timer(const size_t capacity = 1000000)
  {
    data.reserve(capacity);
  }
  
  void tic()
  {
    timestamp = std::chrono::high_resolution_clock::now();
  }

  void toc()
  {
    const std::chrono::high_resolution_clock::duration duration =
      std::chrono::high_resolution_clock::now() - timestamp;

    data.push_back(std::chrono::duration_cast<T>(duration).count());
  }

  std::string str()
  {
    const double sum = std::accumulate(data.begin(), data.end(), 0.0);
    const double sumsum = std::inner_product(data.begin(), data.end(), data.begin(), 0.0);

    const double mean = sum / data.size();
    const double stdev = std::sqrt(sumsum / data.size() - mean * mean);

    std::ostringstream result;
    result.precision(3);
    result << mean << " ± " << stdev;

    return result.str();
  }

private:

  std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;
  std::vector<double> data;

};
