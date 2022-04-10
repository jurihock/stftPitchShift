#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include <anyoption/anyoption.h>
#include <smb/smbPitchShift.h>

#include <StftPitchShift/IO.h>
#include <StftPitchShift/StftPitchShift.h>
#include <StftPitchShift/Version.h>

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

  args.setFlag('h');
  args.setFlag("help");
  args.setFlag("version");
  args.setFlag("smb");
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

  std::string infile = "";
  std::string outfile = "";

  std::vector<float> factors = { 1 };
  float quefrency = 0;

  int framesize = 1024;
  int hoprate = 32;

  try
  {
    if (args.getFlag("smb"))
    {
      smb = true;
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
      quefrency = std::stof(args.getValue("quefrency")) * 1e-3;
    }

    if (args.getValue("window") || args.getValue('w'))
    {
      framesize = std::stoi(args.getValue("window"));
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
    float samplerate;

    std::vector<float> indata, outdata;

    IO::read(infile, indata, samplerate);

    outdata.resize(indata.size());

    if (smb)
    {
      for (auto factor : factors)
      {
        smbPitchShift(
          factor,
          indata.size(),
          framesize,
          hoprate,
          samplerate,
          indata.data(),
          outdata.data());
        
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
        samplerate);

      stft.shiftpitch(
        indata,
        outdata,
        factors,
        quefrency);
    }

    const float min = -1.0f + std::numeric_limits<float>::epsilon();
    const float max = +1.0f - std::numeric_limits<float>::epsilon();

    for (size_t i = 0; i < outdata.size(); ++i)
    {
      outdata[i] = std::min(std::max(outdata[i], min), max);
    }

    IO::write(outfile, outdata, samplerate);
  }
  catch (const std::exception& exception)
  {
    std::cerr << exception.what() << std::endl;
    return NOK;
  }

  return OK;
}
