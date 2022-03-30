#include <Vocoder.h>

Vocoder::Vocoder(const size_t framesize, const size_t hopsize, const float samplerate) :
  framesize(framesize),
  hopsize(hopsize),
  stft_freq_inc((double)samplerate / (double)framesize),
  stft_phase_inc(PI2 / ((double)framesize / (double)hopsize))
{
  encode_phase_buffer.resize(framesize / 2 + 1);
  decode_phase_buffer.resize(framesize / 2 + 1);
}

void Vocoder::encode(std::vector<std::complex<float>>& dft)
{
  double magnitude,
         frequency,
         phase,
         delta,
         j;

  for (size_t i = 0; i < dft.size(); ++i)
  {
    magnitude = std::abs(dft[i]);
    phase = std::arg(dft[i]);

    delta = phase - encode_phase_buffer[i];
    encode_phase_buffer[i] = phase;

    j = wrap(delta - i * stft_phase_inc) / stft_phase_inc;

    frequency = (i + j) * stft_freq_inc;

    dft[i].real(magnitude);
    dft[i].imag(frequency);
  }
}

void Vocoder::decode(std::vector<std::complex<float>>& dft)
{
  double magnitude,
         frequency,
         phase,
         delta,
         j;

  for (size_t i = 0; i < dft.size(); ++i)
  {
    magnitude = dft[i].real();
    frequency = dft[i].imag();

    j = (frequency - i * stft_freq_inc) / stft_freq_inc;

    delta = (i + j) * stft_phase_inc;

    decode_phase_buffer[i] += delta;
    phase = decode_phase_buffer[i];

    dft[i] = std::polar(magnitude, phase);
  }
}
