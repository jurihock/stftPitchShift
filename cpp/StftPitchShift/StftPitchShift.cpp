#include <StftPitchShift.h>

#undef DebugStftPitchShift

#ifdef DebugStftPitchShift
#include <Timer.h>
#endif

#include <Cepstrum.h>
#include <Pitcher.h>
#include <STFT.h>
#include <Vocoder.h>

StftPitchShift::StftPitchShift(
  const size_t framesize,
  const size_t hopsize,
  const float samplerate) :
  framesize(framesize),
  hopsize(hopsize),
  samplerate(samplerate)
{
}

void StftPitchShift::shiftpitch(
  const std::vector<float>& input,
  std::vector<float>& output,
  const std::vector<float>& factors,
  const float quefrency)
{
  #ifdef DebugStftPitchShift
  Timer<std::chrono::microseconds> timer;
  #endif

  STFT stft(framesize, hopsize);
  Vocoder vocoder(framesize, hopsize, samplerate);
  Pitcher pitcher(factors);
  Cepstrum cepstrum(quefrency, samplerate);

  if (quefrency)
  {
    std::vector<float> envelope;

    stft(input, output, [&](std::vector<std::complex<float>>& frame)
    {
      #ifdef DebugStftPitchShift
      timer.tic();
      #endif

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

      #ifdef DebugStftPitchShift
      timer.toc();
      #endif
    });
  }
  else
  {
    stft(input, output, [&](std::vector<std::complex<float>>& frame)
    {
      #ifdef DebugStftPitchShift
      timer.tic();
      #endif

      vocoder.encode(frame);
      pitcher.shiftpitch(frame);
      vocoder.decode(frame);

      #ifdef DebugStftPitchShift
      timer.toc();
      #endif
    });
  }

  #ifdef DebugStftPitchShift
  std::cout << timer.str() << std::endl;
  #endif
}