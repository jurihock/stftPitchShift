#include <emscripten.h>

#include <StftPitchShift/CLI.h>
#include <StftPitchShift/StftPitchShift.h>
#include <StftPitchShift/Version.h>

using namespace stftpitchshift;

void INFO(const std::string& message)
{
  EM_ASM({ console.info(UTF8ToString($0)); }, message.c_str());
}

void ERROR(const std::string& message)
{
  EM_ASM({ console.error(UTF8ToString($0)); }, message.c_str());
}

EM_JS(void, shiftpitch_version, (),
{
  const size = Module._shiftpitch_version_str(0, 0);
  const data = Module._malloc(size + 1);

  Module._shiftpitch_version_str(data, size);
  const value = UTF8ToString(data);
  Module._free(data);

  return value;
});

EM_JS(void, shiftpitch_help, (),
{
  const size = Module._shiftpitch_help_str(0, 0);
  const data = Module._malloc(size + 1);

  Module._shiftpitch_help_str(data, size);
  const value = UTF8ToString(data);
  Module._free(data);

  return value;
});

EM_JS(void, shiftpitch, (int buffer, int args),
{
  console.assert(typeof(buffer) === 'object', buffer);
  console.assert(Object.prototype.toString.call(buffer).slice(8, -1) === 'AudioBuffer', buffer);
  console.assert((typeof(args) === 'undefined') || (typeof(args) === 'string'), args);

  const newbuffer = new AudioBuffer({
    sampleRate: buffer.sampleRate,
    length: buffer.length,
    numberOfChannels: buffer.numberOfChannels
  });

  const samplerate = buffer.sampleRate;
  const samples = buffer.length;
  const channels = buffer.numberOfChannels;

  const meminput = Module._malloc(samples * Float32Array.BYTES_PER_ELEMENT);
  const memoutput = Module._malloc(samples * Float32Array.BYTES_PER_ELEMENT);

  const input = new Float32Array(Module.HEAPF32.buffer, meminput, samples);
  const output = new Float32Array(Module.HEAPF32.buffer, memoutput, samples);

  const memargs = allocateUTF8(args ? args : "");

  for (let channel = 0; channel < channels; channel++)
  {
    buffer.copyFromChannel(input, channel);

    if (!Module._shiftpitch_f32(samplerate, samples, meminput, memoutput, memargs))
    {
      break;
    }

    newbuffer.copyToChannel(output, channel);
  }

  Module._free(memargs);
  Module._free(meminput);
  Module._free(memoutput);

  return newbuffer;
});

extern "C"
{
  int EMSCRIPTEN_KEEPALIVE shiftpitch_version_str(char* data, int size)
  {
    const std::string value = StftPitchShiftVersion;
    value.copy(data, size);
    return value.size();
  }

  int EMSCRIPTEN_KEEPALIVE shiftpitch_help_str(char* data, int size)
  {
    const std::string value = CLI("-h").usage();
    value.copy(data, size);
    return value.size();
  }

  bool EMSCRIPTEN_KEEPALIVE shiftpitch_f32(double samplerate, int samples, float* input, float* output, char* args)
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
        std::get<1>(cli.framesize) / cli.hoprate,
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
