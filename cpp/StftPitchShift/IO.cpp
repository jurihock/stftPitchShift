#include <IO.h>

#define DR_WAV_IMPLEMENTATION
#include <dr_libs/dr_wav.h>

void IO::read(const std::string& path, std::vector<float>& data, float& samplerate, size_t& channels)
{
  struct
  {
    drwav_uint32 channels;
    drwav_uint32 samplerate;
    drwav_uint64 samples;
  }
  wav;

  float* rawdata = drwav_open_file_and_read_pcm_frames_f32(
    path.c_str(), &wav.channels, &wav.samplerate, &wav.samples, nullptr);

  if (rawdata == nullptr)
  {
    throw std::runtime_error(
      "Unable to read \"" + path + "\"!");
  }

  if (wav.channels == 1)
  {
    data.assign(rawdata, rawdata + wav.samples);
  }
  else if (wav.channels == 2)
  {
    data.resize(wav.samples * wav.channels);
    split(data.size(), rawdata, data.data());
  }
  else
  {
    drwav_free(rawdata, nullptr);

    throw std::runtime_error(
      "Unsupported number of channels " + std::to_string(wav.channels) + " in \"" + path + "\"!");
  }

  drwav_free(rawdata, nullptr);

  samplerate = static_cast<float>(wav.samplerate);
  channels = static_cast<size_t>(wav.channels);
}

void IO::write(const std::string& path, const std::vector<float>& data, const float samplerate, const size_t channels)
{
  std::vector<drwav_int32> rawdata(data.size());

  if (channels == 1)
  {
    drwav_f32_to_s32(rawdata.data(), data.data(), data.size());
  }
  else if (channels == 2)
  {
    std::vector<float> buffer(data.size());
    merge(data.size(), data.data(), buffer.data());
    drwav_f32_to_s32(rawdata.data(), buffer.data(), buffer.size());
  }
  else
  {
    throw std::runtime_error(
      "Unsupported number of channels " + std::to_string(channels) + " for \"" + path + "\"!");
  }

  drwav wav;
  drwav_data_format format;

  format.container = drwav_container_riff;
  format.format = DR_WAVE_FORMAT_PCM;
  format.bitsPerSample = sizeof(drwav_uint32) * 8;
  format.channels = static_cast<drwav_uint32>(channels);
  format.sampleRate = static_cast<drwav_uint32>(samplerate);

  const drwav_uint64 samples = rawdata.size() / channels;

  if (drwav_init_file_write(&wav, path.c_str(), &format, nullptr) != DRWAV_TRUE)
  {
    throw std::runtime_error(
      "Unable to write \"" + path + "\"!");
  }

  if (drwav_write_pcm_frames(&wav, samples, rawdata.data()) != samples)
  {
    throw std::runtime_error(
      "Unable to write \"" + path + "\"!");
  }
  
  if (drwav_uninit(&wav) != DRWAV_SUCCESS)
  {
    throw std::runtime_error(
      "Unable to write \"" + path + "\"!");
  }
}

void IO::split(const size_t size, const float* input, float* const output)
{
  const size_t halfsize = size / 2;

  for (size_t i = 0; i < halfsize; ++i)
  {
    output[i] = input[i * 2];
    output[i + halfsize] = input[i * 2 + 1];
  }
}

void IO::merge(const size_t size, const float* input, float* const output)
{
  const size_t halfsize = size / 2;

  for (size_t i = 0; i < halfsize; ++i)
  {
    output[i * 2] = input[i];
    output[i * 2 + 1] = input[i + halfsize];
  }
}
