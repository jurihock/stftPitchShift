#include <iostream>

#include <smb/smbPitchShift.h>
#include <IO.h>

int main()
{
  const std::string infile = "voice.wav";
  const std::string outfile = "test.wav";

  // -----------------------------------------------------------------------------------------------------------------

  std::vector<float> indata, outdata;
  float samplerate;

  IO::read(infile, indata, samplerate);

  outdata.resize(indata.size());

  // -----------------------------------------------------------------------------------------------------------------

  const float smbPitchShiftFactor = 1;
  const long smbNumSampsToProcess = static_cast<long>(indata.size());
  const long smbFftFrameSize = 1 * 1024;
  const long smbOsamp = 32;
  const float smbSampleRate = static_cast<float>(samplerate);

  smbPitchShift(smbPitchShiftFactor, smbNumSampsToProcess, smbFftFrameSize, smbOsamp, smbSampleRate, indata.data(), outdata.data());

  // -----------------------------------------------------------------------------------------------------------------

  IO::write(outfile, outdata, samplerate);

  return 0;
}
