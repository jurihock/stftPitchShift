#pragma once

#include <string>
#include <vector>

class IO
{

public:

  static void clip(std::vector<float>& data);
  static void read(const std::string& path, std::vector<float>& data, float& samplerate, size_t& channels);
  static void write(const std::string& path, const std::vector<float>& data, const float samplerate, const size_t channels);

private:

  static void split(const size_t size, const float* input, float* const output);
  static void merge(const size_t size, const float* input, float* const output);

};
