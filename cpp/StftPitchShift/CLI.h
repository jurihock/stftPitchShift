#pragma once

#include <StftPitchShift/ETC.h>

#include <cxxopts/cxxopts.hpp>

struct CLI
{

public:

  std::string args = "";
  std::string error = "";

  bool empty = false;
  bool help = false;
  bool version = false;

  bool chronometry = false;
  bool normalization = false;

  std::string input = "";
  std::string output = "";

  std::vector<double> factors = { 1 };
  double quefrency = 0;
  double distortion = 1;

  size_t framesize = 1024;
  size_t hoprate = 32;

  CLI(int argc, char** argv) :
    args(join(std::vector<std::string>(argv, argv + argc), ' '))
  {
    parse(argc, argv, true);
  }

  CLI(const std::string& args) :
    args(args)
  {
    std::vector<std::string> strings = split(args, ' ');
    std::vector<char*> chars;

    strings.insert(strings.begin(), "stftpitchshift");

    for (std::string& str : strings)
    {
      chars.push_back(const_cast<char*>(str.c_str()));
    }

    chars.push_back(0);

    parse(chars.size(), &chars.front(), false);
  }

  std::string dump()
  {
    std::stringstream buffer;

    buffer << "args: " << args << std::endl;
    buffer << "error: " << error << std::endl;

    buffer << "empty: " << empty << std::endl;
    buffer << "help: " << help << std::endl;
    buffer << "version: " << version << std::endl;

    buffer << "chronometry: " << chronometry << std::endl;
    buffer << "normalization: " << normalization << std::endl;

    buffer << "input: " << input << std::endl;
    buffer << "output: " << output << std::endl;

    buffer << "factors: " << join(factors, ',') << std::endl;
    buffer << "quefrency: " << quefrency << std::endl;
    buffer << "distortion: " << distortion << std::endl;
    buffer << "framesize: " << framesize << std::endl;
    buffer << "hoprate: " << hoprate << std::endl;

    return buffer.str();
  }

  std::string usage()
  {
    return options.help();
  }

private:

  cxxopts::Options options = cxxopts::Options("stftpitchshift", "STFT based pitch and timbre shifting");

  void parse(int argc, char** argv, bool io)
  {
    options.add_options()
      ("h,help", "print this help")
      ("version", "print version number")
      ("c,chrono", "enable runtime measurements");

    if (io)
    {
      options.add_options("file")
        ("i,input", "input .wav file name", cxxopts::value<std::string>())
        ("o,output", "output .wav file name", cxxopts::value<std::string>());
    }

    options.add_options("pitch shifting")
      ("p,pitch", "fractional pitch shifting factors separated by comma", cxxopts::value<std::string>()->default_value("1.0"))
      ("q,quefrency", "optional formant lifter quefrency in milliseconds", cxxopts::value<std::string>()->default_value("0.0"))
      ("t,timbre", "fractional timbre shifting factor related to -q", cxxopts::value<std::string>()->default_value("1.0"))
      ("r,rms", "enable spectral rms normalization");

    options.add_options("stft")
      ("w,window", "stft window size", cxxopts::value<std::string>()->default_value("1024"))
      ("v,overlap", "stft window overlap", cxxopts::value<std::string>()->default_value("32"));

    const auto args = options.parse(argc, argv);

    try
    {
      if (argc < 2)
      {
        empty = true;
      }

      if (args.count("help"))
      {
        help = true;
      }

      if (args.count("version"))
      {
        version = true;
      }

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
        input = args["input"].as<std::string>();
      }

      if (args.count("output"))
      {
        output = args["output"].as<std::string>();
      }

      if (args.count("pitch"))
      {
        const std::vector<std::string> values = split(
          args["pitch"].as<std::string>(), ',');

        if (!values.empty())
        {
          std::set<double> distinct;

          for (const std::string& value : values)
          {
            double factor;

            if (semicent(value))
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
        quefrency = std::stod(args["quefrency"].as<std::string>()) * 1e-3;
      }

      if (args.count("timbre"))
      {
        const std::string value = args["timbre"].as<std::string>();

        if (semicent(value))
        {
          distortion = semitone(value) * cent(value);
        }
        else
        {
          distortion = std::stod(value);
        }
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
      error = exception.what();
    }
  }

};
