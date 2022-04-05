#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

#include <anyoption/anyoption.h>
#include <smb/smbPitchShift.h>

#include <IO.h>
#include <Resampler.h>
#include <STFT.h>
#include <Vocoder.h>

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

  args.addUsage("Usage:");
  args.addUsage("");
  args.addUsage("-h  --help     print this help");
  args.addUsage("");
  args.addUsage("-i  --input    input .wav file name");
  args.addUsage("-o  --output   output .wav file name");
  args.addUsage("");
  args.addUsage("-p  --pitch    fractional pitch shifting factors separated by comma");
  args.addUsage("               (default 1.0)");
  args.addUsage("");
  args.addUsage("-w  --window   sfft window size");
  args.addUsage("               (default 1024)");
  args.addUsage("");
  args.addUsage("-v  --overlap  stft window overlap");
  args.addUsage("               (default 32)");
  args.addUsage("");
  args.addUsage("    --smb      enable original algorithm");

  args.setFlag("help");
  args.setFlag('h');
  args.setFlag('?');
  args.setFlag("smb");
  args.setOption("input", 'i');
  args.setOption("output", 'o');
  args.setOption("pitch", 'p');
  args.setOption("window", 'w');
  args.setOption("overlap", 'v');

  args.processCommandArgs(argc, argv);

  if (!args.hasOptions() || args.getFlag("help") || args.getFlag('h') || args.getFlag('?'))
  {
    args.printUsage();
    return 0;
  }

  bool smb = false;

  std::string infile = "";
  std::string outfile = "";

  std::vector<float> factors = { 1 };

  int framesize = 1024;
  int hoprate = 32;

  if (args.getFlag("smb"))
  {
    smb = true;
  }

  if (args.getValue("input") || args.getValue('i'))
  {
    infile = args.getValue("input");
  }

  if (args.getValue("output") || args.getValue('o'))
  {
    outfile = args.getValue("output");
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

  if (args.getValue("window") || args.getValue('w'))
  {
    framesize = std::stoi(args.getValue("window"));
  }

  if (args.getValue("overlap") || args.getValue('v'))
  {
    hoprate = std::stoi(args.getValue("overlap"));
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
      STFT stft(framesize, framesize / hoprate);
      Vocoder vocoder(framesize, framesize / hoprate, samplerate);

      std::map<float, Resampler> resample;
      std::map<float, std::vector<std::complex<float>>> buffer;
      std::vector<float> mask;
      bool prepare = true;

      stft(indata, outdata, [&](std::vector<std::complex<float>>& frame)
      {
        if (prepare)
        {
          for (float factor : factors)
          {
            resample[factor] = Resampler(factor);
            buffer[factor].resize(frame.size());
          }
          
          mask.resize(frame.size());
          
          prepare = false;
        }
        
        vocoder.encode(frame);

        for (float factor : factors)
        {
          resample[factor].linear(frame, buffer[factor]);
          
          for (size_t i = 0; i < frame.size(); ++i)
          {
            buffer[factor][i].imag(buffer[factor][i].imag() * factor);
          }
        }

        for (size_t i = 0; i < frame.size(); ++i)
        {
          float maximum = std::numeric_limits<float>::lowest();

          for (float factor : factors)
          {
            const float current = buffer[factor][i].real();

            if (current > maximum)
            {
              maximum = current;
              mask[i] = factor;
            }
          }
        }
        
        for (size_t i = 0; i < frame.size(); ++i)
        {
          frame[i] = buffer[mask[i]][i];
        }

        vocoder.decode(frame);
      });
    }

    IO::write(outfile, outdata, samplerate);
  }
  catch (const std::exception& exception)
  {
    std::cerr << exception.what() << std::endl;
    return 1;
  }

  return 0;
}
