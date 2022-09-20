#include <emscripten.h>

#include <StftPitchShift/CLI.h>
#include <StftPitchShift/StftPitchShift.h>
#include <StftPitchShift/Version.h>

using namespace stftpitchshift;

const int api = 1; // current wasm api version

void INFO(const std::string& message)
{
  EM_ASM({ console.info(UTF8ToString($0)); }, message.c_str());
}

void ERROR(const std::string& message)
{
  EM_ASM({ console.error(UTF8ToString($0)); }, message.c_str());
}

extern "C"
{
  int EMSCRIPTEN_KEEPALIVE version(int index)
  {
    std::vector<std::string> version = split(StftPitchShiftVersion, '.');

    if (index < 0)
    {
      return api;
    }

    if (index >= version.size())
    {
      return 0;
    }

    return std::stoi(version.at(index));
  }

  bool EMSCRIPTEN_KEEPALIVE shiftpitch(double samplerate, int samples, float* input, float* output, char* args)
  {
    CLI cli(args);

    if (cli.help)
    {
      INFO(cli.usage());
      return false;
    }

    if (cli.version)
    {
      INFO(StftPitchShiftVersion);
      return false;
    }

    try
    {
      StftPitchShift stft(
        cli.framesize,
        cli.hoprate ? cli.framesize / cli.hoprate : 1,
        samplerate,
        cli.normalization,
        cli.chronometry);

      stft.shiftpitch(
        samples,
        input,
        output,
        cli.factors,
        cli.quefrency,
        cli.distortion);
    }
    catch (const std::exception& exception)
    {
      ERROR(exception.what());
      return false;
    }

    return true;
  }
}
