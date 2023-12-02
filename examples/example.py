from stftpitchshift import StftPitchShift

pitchshifter = StftPitchShift(44100, 1024, 256)

x = [0] * 44100
y = pitchshifter.shiftpitch(x, 1)
