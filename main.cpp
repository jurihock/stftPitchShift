#include <iostream>

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

#include "smbPitchShift.h"

int main()
{
  std::string infilename = "voice.wav";
  std::string outfilename = "test.wav";

  float* indata = nullptr;
  float* outdata = nullptr;

  // -----------------------------------------------------------------------------------------------------------------

  unsigned int wavChannels;
  unsigned int wavSampleRate;
  drwav_uint64 wavSamples;

  indata = drwav_open_file_and_read_pcm_frames_f32(infilename.c_str(), &wavChannels, &wavSampleRate, &wavSamples, nullptr);

  if (indata == nullptr)
  {
    std::cerr << "Unable to read " << infilename.c_str() << std::endl;
    return 1;
  }

  if (wavChannels != 1)
  {
    std::cerr << "Unsupported number of channels " << wavChannels << std::endl;
    return 1;
  }

  outdata = new float[wavChannels * wavSamples];

  // -----------------------------------------------------------------------------------------------------------------

  const float smbPitchShiftFactor = 1;
  const long smbNumSampsToProcess = static_cast<long>(wavSamples);
  const long smbFftFrameSize = 1 * 1024;
  const long smbOsamp = 32;
  const float smbSampleRate = static_cast<float>(wavSampleRate);

  smbPitchShift(smbPitchShiftFactor, smbNumSampsToProcess, smbFftFrameSize, smbOsamp, smbSampleRate, indata, outdata);

  // -----------------------------------------------------------------------------------------------------------------

  drwav wav;

  drwav_data_format wavFormat;
  wavFormat.container = drwav_container_riff;
  wavFormat.format = DR_WAVE_FORMAT_IEEE_FLOAT;
  wavFormat.channels = wavChannels;
  wavFormat.sampleRate = wavSampleRate;
  wavFormat.bitsPerSample = 32;

  const drwav_bool32 wavInitWrite = drwav_init_file_write(&wav, outfilename.c_str(), &wavFormat, nullptr);

  if (wavInitWrite != DRWAV_TRUE)
  {
    std::cerr << "Unable to write " << outfilename.c_str() << std::endl;
    return 1;
  }

  const drwav_uint64 wavSamplesWritten = drwav_write_pcm_frames(&wav, wavSamples, outdata);

  if (wavSamplesWritten != wavSamples)
  {
    std::cerr << "Unable to write " << outfilename.c_str() << std::endl;
    return 1;
  }

  // -----------------------------------------------------------------------------------------------------------------

  drwav_free(indata, nullptr);
  delete[] outdata;

  return 0;
}
