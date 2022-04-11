#pragma once

void smbPitchShift(float pitchShift, long numSampsToProcess, long fftFrameSize, long osamp, float sampleRate, const float* indata, float* const outdata);

void smbPitchShift(float pitchShift, size_t numSampsToProcess, size_t fftFrameSize, size_t osamp, float sampleRate, const float* indata, float* const outdata)
{
  smbPitchShift(
    pitchShift,
    static_cast<long>(numSampsToProcess),
    static_cast<long>(fftFrameSize),
    static_cast<long>(osamp),
    sampleRate,
    indata,
    outdata);
}
