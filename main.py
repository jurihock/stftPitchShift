from IO import read, write
from STFT import stft, istft
from Vocoder import encode, decode, resample

import numpy as np


x, sr = read('voice')

factor = 0.5
framesize = 1024
hopsize = framesize // 32

frames = stft(x, framesize, hopsize)
frames = encode(frames, framesize, hopsize, sr)

for i in range(len(frames)):

    envelope = np.real(frames[i])
    frequencies = np.imag(frames[i])

    envelope = resample(envelope, factor)
    frequencies = resample(frequencies, factor)

    frames[i] = envelope + 1j * frequencies * factor

frames = decode(frames, framesize, hopsize, sr)

y = istft(frames, framesize, hopsize)

write('test', y, sr)
