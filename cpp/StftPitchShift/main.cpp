#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <anyoption/anyoption.h>
#include <smb/smbPitchShift.h>

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
  AnyOption args;

  args.addUsage("Usage: stftpitchshift [OPTIONS]");
  args.addUsage("");
  args.addUsage("  STFT based multi pitch shifting with optional formant preservation");
  args.addUsage("");
  args.addUsage("Options:");
  args.addUsage("");
  args.addUsage("  -h  --help       print this help");
  args.addUsage("      --version    print version number");
  args.addUsage("");
  args.addUsage("  -i  --input      input .wav file name");
  args.addUsage("  -o  --output     output .wav file name");
  args.addUsage("");
  args.addUsage("  -p  --pitch      fractional pitch shifting factors separated by comma");
  args.addUsage("                   (default 1.0)");
  args.addUsage("");
  args.addUsage("  -q  --quefrency  optional formant lifter quefrency in milliseconds");
  args.addUsage("                   (default 0.0)");
  args.addUsage("");
  args.addUsage("  -w  --window     sfft window size");
  args.addUsage("                   (default 1024)");
  args.addUsage("");
  args.addUsage("  -v  --overlap    stft window overlap");
  args.addUsage("                   (default 32)");
  args.addUsage("");
  args.addUsage("      --smb        enable original smb algorithm");
  args.addUsage("      --chrono     enable runtime measurements");

  args.setFlag('h');
  args.setFlag("help");
  args.setFlag("version");
  args.setFlag("smb");
  args.setFlag("chrono");

  args.setOption("input", 'i');
  args.setOption("output", 'o');
  args.setOption("pitch", 'p');
  args.setOption("quefrency", 'q');
  args.setOption("window", 'w');
  args.setOption("overlap", 'v');

  args.processCommandArgs(argc, argv);

  if (!args.hasOptions())
  {
    args.printUsage();
    return NOK;
  }

  if (args.getFlag("help") || args.getFlag('h'))
  {
    args.printUsage();
    return OK;
  }

  if (args.getFlag("version"))
  {
    std::cout << StftPitchShiftVersion << std::endl;
    return OK;
  }

  bool smb = false;
  bool chronometry = false;

  std::string infile = "";
  std::string outfile = "";

  std::vector<double> factors = { 1 };
  double quefrency = 0;

  size_t framesize = 1024;
  size_t hoprate = 32;

  try
  {
    if (args.getFlag("smb"))
    {
      smb = true;
    }

    if (args.getFlag("chrono"))
    {
      chronometry = true;
    }

    if (args.getValue("input") || args.getValue('i'))
    {
      infile = args.getValue("input");
    }
    else
    {
      std::cerr << "Missing input file name!" << std::endl;
      return NOK;
    }

    if (args.getValue("output") || args.getValue('o'))
    {
      outfile = args.getValue("output");
    }
    else
    {
      std::cerr << "Missing output file name!" << std::endl;
      return NOK;
    }

    if (args.getValue("pitch") || args.getValue('p'))
    {
      const auto values = split(args.getValue("pitch"), ',');

      if (!values.empty())
      {
        factors.clear();

        for (const auto& value : values)
        {
          factors.push_back(std::stof(value));
        }
      }
    }

    if (args.getValue("quefrency") || args.getValue('q'))
    {
      quefrency = std::stof(args.getValue("quefrency")) * 1e-3f;
    }

    if (args.getValue("window") || args.getValue('w'))
    {
      const std::string value = args.getValue("window");

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

    if (args.getValue("overlap") || args.getValue('v'))
    {
      hoprate = std::stoi(args.getValue("overlap"));
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

      if (smb)
      {
        for (const double factor : factors)
        {
          smbPitchShift(
            factor,
            size,
            framesize,
            hoprate,
            samplerate,
            input,
            output);

          for (size_t i = 0; i < outdata.size(); ++i)
          {
            outdata[i] /= factors.size();
          }
        }
      }
      else
      {
        StftPitchShift stft(
          framesize,
          framesize / hoprate,
          samplerate,
          chronometry);

        stft.shiftpitch(
          size,
          input,
          output,
          factors,
          quefrency);
      }
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
