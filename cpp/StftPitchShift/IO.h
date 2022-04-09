#pragma once

#include <string>
#include <vector>

class IO
{

public:

  static void read(const std::string& path, std::vector<float>& data, float& sr);
  static void write(const std::string& path, const std::vector<float>& data, const float sr);

};
