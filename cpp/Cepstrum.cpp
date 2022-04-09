#include <Cepstrum.h>

#include <pocketfft/pocketfft_hdronly.h>

Cepstrum::Cepstrum(const float quefrency, const float samplerate) :
  quefrency(static_cast<size_t>(quefrency * samplerate))
{
}

void Cepstrum::lifter(const std::vector<std::complex<float>>& dft, std::vector<float>& envelope)
{
  spectrum.resize(dft.size());
  cepstrum.resize((dft.size() - 1) * 2);
  envelope.resize(dft.size());

  for (size_t i = 0; i < spectrum.size(); ++i)
  {
    spectrum[i] = std::log10(dft[i].real());
  }

  ifft(spectrum, cepstrum, 1.0f);
  lowpass(cepstrum, quefrency);
  fft(cepstrum, spectrum, 1.0f / cepstrum.size());

  for (size_t i = 0; i < spectrum.size(); ++i)
  {
    envelope[i] = std::pow(10, spectrum[i].real());
  }
}

void Cepstrum::lowpass(std::vector<float>& cepstrum, const size_t quefrency)
{
  for (size_t i = 1; i < std::min(quefrency, cepstrum.size()); ++i)
  {
    cepstrum[i] *= 2;
  }

  for (size_t i = quefrency + 1; i < cepstrum.size(); ++i)
  {
    cepstrum[i] = 0;
  }
}

void Cepstrum::fft(const std::vector<float>& frame, std::vector<std::complex<float>>& dft, const float scale)
{
  pocketfft::r2c(
    { frame.size() },
    { sizeof(float) },
    { sizeof(std::complex<float>) },
    0,
    true,
    frame.data(),
    dft.data(),
    scale);
}

void Cepstrum::ifft(const std::vector<std::complex<float>>& dft, std::vector<float>& frame, const float scale)
{
  pocketfft::c2r(
    { frame.size() },
    { sizeof(std::complex<float>) },
    { sizeof(float) },
    0,
    false,
    dft.data(),
    frame.data(),
    scale);
}
