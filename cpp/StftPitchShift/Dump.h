#pragma once

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace stftpitchshift
{
  class Dump
  {

  public:

    Dump(const std::string& filename, const size_t minindex = 0, const size_t maxindex = 0) :
      filename(filename),
      minindex(minindex),
      maxindex(maxindex),
      fileindex(0)
    {
    }

    template<class T>
    void operator()(const std::vector<T>& data)
    {
      if (fileindex < minindex)
      {
        ++fileindex;
        return;
      }

      if (fileindex > maxindex && maxindex > minindex)
      {
        ++fileindex;
        return;
      }

      std::stringstream filepath;

      filepath << filename;
      filepath << ".";
      filepath << std::setw(10) << std::setfill('0') << fileindex;
      filepath << ".raw";

      std::ofstream file(
        filepath.str(),
        std::ios::out | std::ios::binary);

      file.write(
        reinterpret_cast<const char*>(data.data()),
        data.size() * sizeof(T));

      ++fileindex;
    }

  private:

    const std::string filename;

    const size_t minindex;
    const size_t maxindex;

    size_t fileindex;

  };
}
