#pragma once

void smbPitchShift(float pitchShift, long numSampsToProcess, long fftFrameSize, long osamp, float sampleRate, const float* indata, float* const outdata);

void smbPitchShift(double pitchShift, size_t numSampsToProcess, size_t fftFrameSize, size_t osamp, double sampleRate, const float* indata, float* const outdata)
{
  smbPitchShift(
    static_cast<float>(pitchShift),
    static_cast<long>(numSampsToProcess),
    static_cast<long>(fftFrameSize),
    static_cast<long>(osamp),
    static_cast<float>(sampleRate),
    indata,
    outdata);
}

void smbPitchShift(double pitchShift, size_t numSampsToProcess, size_t fftFrameSize, size_t osamp, double sampleRate, const double* indata, double* const outdata)
{
  std::vector<float> inbuffer(numSampsToProcess);
  std::vector<float> outbuffer(numSampsToProcess);

  std::copy(indata, indata + numSampsToProcess, inbuffer.begin());

  smbPitchShift(
    static_cast<float>(pitchShift),
    static_cast<long>(numSampsToProcess),
    static_cast<long>(fftFrameSize),
    static_cast<long>(osamp),
    static_cast<float>(sampleRate),
    inbuffer.data(),
    outbuffer.data());

  std::copy(outbuffer.begin(), outbuffer.end(), outdata);
}
