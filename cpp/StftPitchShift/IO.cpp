#include <StftPitchShift/IO.h>

#include <dr_libs/dr_wav.h>

using namespace stftpitchshift;

void IO::read(const std::string& path, std::vector<float>& data, double& samplerate, size_t& channels)
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

  samplerate = static_cast<double>(wav.samplerate);
  channels = static_cast<size_t>(wav.channels);
}

void IO::read(const std::string& path, std::vector<double>& data, double& samplerate, size_t& channels)
{
  std::vector<float> buffer;

  read(path, buffer, samplerate, channels);

  data.assign(buffer.begin(), buffer.end());
}

void IO::write(const std::string& path, const std::vector<float>& data, const double samplerate, const size_t channels)
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

void IO::write(const std::string& path, const std::vector<double>& data, const double samplerate, const size_t channels)
{
  std::vector<float> buffer;

  buffer.assign(data.begin(), data.end());

  write(path, buffer, samplerate, channels);
}
