#include <StftPitchShift/StftPitchShift.h>

using namespace stftpitchshift;

int main()
{
  StftPitchShift pitchshifter(1024, 256, 44100);

  std::vector<float> x(44100);
  std::vector<float> y(x.size());

  pitchshifter.shiftpitch(x, y, 1);

  return 0;
}
