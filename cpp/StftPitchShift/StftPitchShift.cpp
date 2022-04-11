#include <StftPitchShift.h>

#include <Cepstrum.h>
#include <Pitcher.h>
#include <STFT.h>
#include <Vocoder.h>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

StftPitchShift::StftPitchShift(
  const size_t framesize,
  const size_t hopsize,
  const float samplerate,
  const bool chronometry) :
  framesize(framesize),
  hopsize(hopsize),
  samplerate(samplerate)
{
  debug.chronometry = chronometry;
  debug.dump = false;
  debug.filename = "";
  debug.fileindex = 0;
}

void StftPitchShift::shiftpitch(
  const std::vector<float>& input,
  std::vector<float>& output,
  const float factor,
  const float quefrency)
{
  const std::vector<float> factors = { factor };

  shiftpitch(
    input.size(),
    input.data(),
    output.data(),
    factors,
    quefrency);
}

void StftPitchShift::shiftpitch(
  const size_t size,
  const float* input,
  float* const output,
  const float factor,
  const float quefrency)
{
  const std::vector<float> factors = { factor };

  shiftpitch(
    size,
    input,
    output,
    factors,
    quefrency);
}

void StftPitchShift::shiftpitch(
  const std::vector<float>& input,
  std::vector<float>& output,
  const std::vector<float>& factors,
  const float quefrency)
{
  shiftpitch(
    input.size(),
    input.data(),
    output.data(),
    factors,
    quefrency);
}

void StftPitchShift::shiftpitch(
  const size_t size,
  const float* input,
  float* const output,
  const std::vector<float>& factors,
  const float quefrency)
{
  STFT stft(framesize, hopsize, debug.chronometry);
  Vocoder vocoder(framesize, hopsize, samplerate);
  Pitcher pitcher(factors);
  Cepstrum cepstrum(quefrency, samplerate);

  debug.fileindex = 0;

  if (quefrency)
  {
    std::vector<float> envelope;

    stft(size, input, output, [&](std::vector<std::complex<float>>& frame)
    {
      vocoder.encode(frame);

      cepstrum.lifter(frame, envelope);

      for (size_t i = 0; i < frame.size(); ++i)
      {
        frame[i].real(frame[i].real() / envelope[i]);
      }

      pitcher.shiftpitch(frame);

      for (size_t i = 0; i < frame.size(); ++i)
      {
        frame[i].real(frame[i].real() * envelope[i]);
      }

      vocoder.decode(frame);
    });
  }
  else
  {
    stft(size, input, output, [&](std::vector<std::complex<float>>& frame)
    {
      vocoder.encode(frame);
      pitcher.shiftpitch(frame);
      vocoder.decode(frame);
    });
  }
}

void StftPitchShift::dump(const std::string& filename, const size_t fileindex, const std::vector<float>& data)
{
  std::stringstream filepath;

  filepath << filename;
  filepath << ".";
  filepath << std::setw(10) << std::setfill('0') << fileindex;
  filepath << ".raw";

  std::ofstream file(filepath.str(), std::ios::out | std::ios::binary);

  file.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(float));
  file.close();
}

void StftPitchShift::dump(const std::string& filename, const size_t fileindex, const std::vector<std::complex<float>>& data)
{
  std::vector<float> buffer(data.size());

  std::transform(data.begin(), data.end(), buffer.begin(),
    [](const std::complex<float>& value) { return std::real(value); });

  dump(filename + ".real", fileindex, buffer);

  std::transform(data.begin(), data.end(), buffer.begin(),
    [](const std::complex<float>& value) { return std::imag(value); });

  dump(filename + ".imag", fileindex, buffer);
}
