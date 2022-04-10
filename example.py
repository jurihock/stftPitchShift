from stftpitchshift import StftPitchShift

pitchshifter = StftPitchShift(1024, 256, 44100)

x = [0] * 44100
y = pitchshifter.shiftpitch(x, 1)
