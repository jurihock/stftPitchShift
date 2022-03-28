#include <iostream>

#include <IO.h>
#include <STFT.h>
#include <Vocoder.h>

#include <smb/smbPitchShift.h>

int main()
{
  const std::string infile = "voice.wav";
  const std::string outfile = "test.wav";

  const float factor = 0.5;

  const int framesize = 1 * 1024;
  const int hoprate = 32;
  const int hopsize = framesize / hoprate;

  float samplerate;

  std::vector<float> indata, outdata;

  IO::read(infile, indata, samplerate);

  outdata.resize(indata.size());

  if (true)
  {
    STFT stft(framesize, hopsize);
    Vocoder vocoder(framesize, hopsize, samplerate);

    stft(indata, outdata, [&](std::vector<std::complex<float>>& src)
    {
      std::vector<std::complex<float>> dst(src.size());

      vocoder.encode(src);

      for (size_t i = 0; i < src.size(); ++i)
      {
        const size_t j = i * factor;

        if (j >= src.size())
          continue;

        dst[j].real(dst[j].real() + std::real(src[i]));
        dst[j].imag(std::imag(src[i]) * factor);
      }

      vocoder.decode(dst);

      src = dst;
    });
  }
  else
  {
    smbPitchShift(
      factor,
      indata.size(),
      framesize,
      hoprate,
      samplerate,
      indata.data(),
      outdata.data());
  }

  IO::write(outfile, outdata, samplerate);

  return 0;
}
