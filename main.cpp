#include <iostream>

#include <anyoption/anyoption.h>
#include <smb/smbPitchShift.h>

#include <IO.h>
#include <STFT.h>
#include <Vocoder.h>

int main(int argc, char** argv)
{
  AnyOption args;

  args.addUsage("Usage:");
  args.addUsage("");
  args.addUsage("-h,?  --help     print this help");
  args.addUsage("");
  args.addUsage("-i    --input    input .wav file name");
  args.addUsage("-o    --output   output .wav file name");
  args.addUsage("");
  args.addUsage("-p    --pitch    pitch shifting factor");
  args.addUsage("                 (default 1.0)");
  args.addUsage("");
  args.addUsage("-w    --window   sfft window size");
  args.addUsage("                 (default 1024)");
  args.addUsage("");
  args.addUsage("-v    --overlap  stft window overlap");
  args.addUsage("                 (default 32)");
  args.addUsage("");
  args.addUsage("      --smb      enable original algorithm");

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

  float factor = 1.0;

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
    factor = std::stof(args.getValue("pitch"));
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
      smbPitchShift(
        factor,
        indata.size(),
        framesize,
        hoprate,
        samplerate,
        indata.data(),
        outdata.data());
    }
    else
    {
      STFT stft(framesize, framesize / hoprate);
      Vocoder vocoder(framesize, framesize / hoprate, samplerate);

      stft(indata, outdata, [&](std::vector<std::complex<float>>& src)
      {
        std::vector<std::complex<float>> dst(src.size());

        vocoder.encode(src);

        for (size_t i = 0; i < src.size(); ++i)
        {
          const size_t j = i * factor;

          if (j >= src.size())
            continue;

          dst[j].real(dst[j].real() + std::real(src[i]));
          dst[j].imag(std::imag(src[i]) * factor);
        }

        vocoder.decode(dst);

        src = dst;
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
