#include <cmath>
#include <functional>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <cxxopts/cxxopts.hpp>

#include <StftPitchShift/IO.h>
#include <StftPitchShift/StftPitchShift.h>
#include <StftPitchShift/Version.h>

using namespace stftpitchshift;

const int OK = 0;
const int NOK = 1;

std::vector<std::string> split(const std::string& value, const char delimiter)
{
  std::vector<std::string> values;

  std::stringstream lines(value);
  std::string line;

  while (std::getline(lines, line, delimiter))
  {
    values.push_back(line);
  }

  return values;
}

int main(int argc, char** argv)
{
  cxxopts::Options options("stftpitchshift", "STFT based multi pitch shifting with optional formant preservation");

  options.add_options()
    ("h,help", "print this help")
    ("version", "print version number")
    ("c,chrono", "enable runtime measurements");

  options.add_options("file")
    ("i,input", "input .wav file name", cxxopts::value<std::string>())
    ("o,output", "output .wav file name", cxxopts::value<std::string>());

  options.add_options("pitch shifting")
    ("p,pitch", "fractional pitch shifting factors separated by comma", cxxopts::value<std::string>()->default_value("1.0"))
    ("q,quefrency", "optional formant lifter quefrency in milliseconds", cxxopts::value<std::string>()->default_value("0.0"))
    ("r,rms", "enable spectral rms normalization");

  options.add_options("stft")
    ("w,window", "sfft window size", cxxopts::value<std::string>()->default_value("1024"))
    ("v,overlap", "stft window overlap", cxxopts::value<std::string>()->default_value("32"));

  const auto args = options.parse(argc, argv);

  if (argc < 2)
  {
    std::cout << options.help() << std::endl;
    return OK;
  }

  if (args.count("help"))
  {
    std::cout << options.help() << std::endl;
    return OK;
  }

  if (args.count("version"))
  {
    std::cout << StftPitchShiftVersion << std::endl;
    return OK;
  }

  bool chronometry = false;
  bool normalization = false;

  std::string infile = "";
  std::string outfile = "";

  std::vector<double> factors = { 1 };
  double quefrency = 0;

  size_t framesize = 1024;
  size_t hoprate = 32;

  try
  {
    if (args.count("chrono"))
    {
      chronometry = true;
    }

    if (args.count("rms"))
    {
      normalization = true;
    }

    if (args.count("input"))
    {
      infile = args["input"].as<std::string>();
    }
    else
    {
      std::cerr << "Missing input file name!" << std::endl;
      std::cerr << "Please specify -h for full usage..." << std::endl;
      return NOK;
    }

    if (args.count("output"))
    {
      outfile = args["output"].as<std::string>();
    }
    else
    {
      std::cerr << "Missing output file name!" << std::endl;
      std::cerr << "Please specify -h for full usage..." << std::endl;
      return NOK;
    }

    if (args.count("pitch"))
    {
      const std::vector<std::string> values = split(
        args["pitch"].as<std::string>(), ',');

      if (!values.empty())
      {
        const auto parse = [](const std::string& value) -> bool
        {
          return (value.front() == '+') || (value.front() == '-') ||
                 (value.front() == '0'  && (value.find('.') == std::string::npos));
        };

        const auto semitone = [](const std::string& value) -> double
        {
          std::regex regex("([+,-]?\\d+){1}([+,-]\\d+){0,1}");
          std::smatch matches;
          std::regex_match(value, matches, regex);
          std::string match = matches[1];
          return std::pow(2, std::stod(match) / 12);
        };

        const auto cent = [](const std::string& value) -> double
        {
          std::regex regex("([+,-]?\\d+){1}([+,-]\\d+){0,1}");
          std::smatch matches;
          std::regex_match(value, matches, regex);
          std::string match = (matches[2] == "") ? std::string("0") : matches[2];
          return std::pow(2, std::stod(match) / 1200);
        };

        std::set<double> distinct;

        for (const std::string& value : values)
        {
          double factor;

          if (parse(value))
          {
            factor = semitone(value) * cent(value);
          }
          else
          {
            factor = std::stod(value);
          }

          distinct.insert(factor);
        }

        factors.assign(distinct.begin(), distinct.end());
      }
    }

    if (args.count("quefrency"))
    {
      quefrency = std::stod(args["quefrency"].as<std::string>()) * 1e-3f;
    }

    if (args.count("window"))
    {
      const std::string value = args["window"].as<std::string>();

      if (value == "1k")
      {
        framesize = 1 * 1024;
      }
      else if (value == "2k")
      {
        framesize = 2 * 1024;
      }
      else if (value == "4k")
      {
        framesize = 4 * 1024;
      }
      else
      {
        framesize = std::stoi(value);
      }
    }

    if (args.count("overlap"))
    {
      hoprate = std::stoi(args["overlap"].as<std::string>());
    }
  }
  catch (const std::exception& exception)
  {
    std::cerr << "Unable to interpret the specified command line options!" << std::endl;
    std::cerr << exception.what() << std::endl;
    return NOK;
  }

  try
  {
    double samplerate;
    size_t channels;

    std::vector<double> indata, outdata;

    IO::read(infile, indata, samplerate, channels);

    outdata.resize(indata.size());

    for (size_t channel = 0; channel < channels; ++channel)
    {
      const size_t size = indata.size() / channels;
      const double* input = indata.data() + channel * size;
      double* const output = outdata.data() + channel * size;

      StftPitchShift stft(
        framesize,
        framesize / hoprate,
        samplerate,
        normalization,
        chronometry);

      stft.shiftpitch(
        size,
        input,
        output,
        factors,
        quefrency);
    }

    IO::clip(outdata);
    IO::write(outfile, outdata, samplerate, channels);
  }
  catch (const std::exception& exception)
  {
    std::cerr << exception.what() << std::endl;
    return NOK;
  }

  return OK;
}
