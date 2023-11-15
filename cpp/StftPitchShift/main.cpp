#include <iostream>

#include <StftPitchShift/CLI.h>
#include <StftPitchShift/IO.h>
#include <StftPitchShift/StftPitchShift.h>
#include <StftPitchShift/Version.h>

using namespace stftpitchshift;

const int OK = 0;
const int NOK = 1;

int main(int argc, char** argv)
{
  CLI cli(argc, argv);

  if (!cli.error.empty())
  {
    std::cerr << "Unable to interpret the specified command line options!" << std::endl;
    std::cerr << cli.error << std::endl;
    return NOK;
  }

  if (cli.empty || cli.help)
  {
    std::cout << cli.usage() << std::endl;
    return OK;
  }

  if (cli.version)
  {
    std::cout << StftPitchShiftVersion << std::endl;
    return OK;
  }

  if (cli.input.empty())
  {
    std::cerr << "Missing input file name!" << std::endl;
    std::cerr << "Please specify -h for full usage..." << std::endl;
    return NOK;
  }

  if (cli.output.empty())
  {
    std::cerr << "Missing output file name!" << std::endl;
    std::cerr << "Please specify -h for full usage..." << std::endl;
    return NOK;
  }

  try
  {
    double samplerate;
    size_t channels;

    std::vector<double> indata, outdata;

    IO::read(cli.input, indata, samplerate, channels);

    outdata.resize(indata.size());

    for (size_t channel = 0; channel < channels; ++channel)
    {
      const size_t size = indata.size() / channels;
      const double* input = indata.data() + channel * size;
      double* const output = outdata.data() + channel * size;

      StftPitchShift stft(
        cli.framesize,
        std::get<1>(cli.framesize) / cli.hoprate,
        samplerate,
        cli.normalization,
        cli.chronometry);

      stft.shiftpitch(
        size,
        input,
        output,
        cli.factors,
        cli.quefrency,
        cli.distortion);
    }

    IO::clip(outdata);
    IO::write(cli.output, outdata, samplerate, channels);
  }
  catch (const std::exception& exception)
  {
    std::cerr << exception.what() << std::endl;
    return NOK;
  }

  return OK;
}
