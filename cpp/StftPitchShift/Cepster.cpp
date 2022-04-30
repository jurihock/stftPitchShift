#include <StftPitchShift/Cepster.h>

Cepster::Cepster(const std::shared_ptr<FFT> fft, const double quefrency, const double samplerate) :
  fft(fft),
  quefrency(static_cast<size_t>(quefrency * samplerate))
{
}

void Cepster::lifter(const std::vector<std::complex<float>>& dft, std::vector<float>& envelope)
{
  spectrum.resize(dft.size());
  cepstrum.resize((dft.size() - 1) * 2);
  envelope.resize(dft.size());

  for (size_t i = 0; i < spectrum.size(); ++i)
  {
    spectrum[i] = std::log10(dft[i].real());
  }

  fft->ifft(spectrum, cepstrum);
  lowpass(cepstrum, quefrency);
  fft->fft(cepstrum, spectrum);

  for (size_t i = 0; i < spectrum.size(); ++i)
  {
    envelope[i] = std::pow(10.0f, spectrum[i].real());
  }
}

void Cepster::lowpass(std::vector<float>& cepstrum, const size_t quefrency)
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
