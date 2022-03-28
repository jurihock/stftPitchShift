#include <IO.h>

#define DR_WAV_IMPLEMENTATION
#include <dr_libs/dr_wav.h>

void IO::read(const std::string& path, std::vector<float>& data, float& sr)
{
  drwav_uint32 channels;
  drwav_uint32 samplerate;
  drwav_uint64 samples;

  float* rawdata = drwav_open_file_and_read_pcm_frames_f32(
    path.c_str(), &channels, &samplerate, &samples, nullptr);

  if (rawdata == nullptr)
  {
    throw std::runtime_error(
      "Unable to read " + path + "!");
  }

  if (channels != 1)
  {
    throw std::runtime_error(
      "Unsupported number of channels " + std::to_string(channels) + " in " + path + "!");
  }

  data.assign(rawdata, rawdata + samples);
  sr = static_cast<float>(samplerate);

  drwav_free(rawdata, nullptr);
}

void IO::write(const std::string& path, const std::vector<float>& data, const float sr)
{
  const float* rawdata = data.data();
  const drwav_uint64 samples = static_cast<drwav_uint64>(data.size());

  drwav wav;

  drwav_data_format format;
  format.container = drwav_container_riff;
  format.format = DR_WAVE_FORMAT_IEEE_FLOAT;
  format.bitsPerSample = 32;
  format.channels = 1;
  format.sampleRate = static_cast<drwav_uint32>(sr);

  if (drwav_init_file_write(&wav, path.c_str(), &format, nullptr) != DRWAV_TRUE)
  {
    throw std::runtime_error(
      "Unable to write " + path + "!");
  }

  if (drwav_write_pcm_frames(&wav, samples, rawdata) != samples)
  {
    throw std::runtime_error(
      "Unable to write " + path + "!");
  }
}
