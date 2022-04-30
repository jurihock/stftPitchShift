#pragma once

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

class IO
{

public:

  static void read(const std::string& path, std::vector<float>& data, double& samplerate, size_t& channels);
  static void read(const std::string& path, std::vector<double>& data, double& samplerate, size_t& channels);

  static void write(const std::string& path, const std::vector<float>& data, const double samplerate, const size_t channels);
  static void write(const std::string& path, const std::vector<double>& data, const double samplerate, const size_t channels);

  template<class T>
  static void clip(std::vector<T>& data)
  {
    const T min = T(-1) + std::numeric_limits<T>::epsilon();
    const T max = T(+1) - std::numeric_limits<T>::epsilon();

    for (size_t i = 0; i < data.size(); ++i)
    {
      data[i] = std::min(std::max(data[i], min), max);
    }
  }

  template<class T>
  static void split(const size_t size, const T* input, T* const output)
  {
    const size_t halfsize = size / 2;

    for (size_t i = 0; i < halfsize; ++i)
    {
      output[i] = input[i * 2];
      output[i + halfsize] = input[i * 2 + 1];
    }
  }

  template<class T>
  static void merge(const size_t size, const T* input, T* const output)
  {
    const size_t halfsize = size / 2;

    for (size_t i = 0; i < halfsize; ++i)
    {
      output[i * 2] = input[i];
      output[i * 2 + 1] = input[i + halfsize];
    }
  }

};
