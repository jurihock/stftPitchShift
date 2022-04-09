from StftPitchShift.Cepstrum import lifter
from StftPitchShift.Pitcher import shiftpitch
from StftPitchShift.STFT import stft, istft, spectrogram
from StftPitchShift.Vocoder import encode, decode

import numpy as np


class StftPitchShift:

    def __init__(self, framesize, hopsize, samplerate):

        self.framesize = framesize
        self.hopsize = hopsize
        self.samplerate = samplerate

    def shiftpitch(self, input, factors = 1, quefrency = 0):

        framesize = self.framesize
        hopsize = self.hopsize
        samplerate = self.samplerate

        factors = np.asarray(factors).flatten()
        quefrency = int(quefrency * samplerate)

        frames = stft(input, framesize, hopsize)

        if quefrency:

            frames = encode(frames, framesize, hopsize, samplerate)

            envelopes = lifter(frames, quefrency)

            frames.real /= envelopes
            frames = shiftpitch(frames, factors)
            frames.real *= envelopes

            frames = decode(frames, framesize, hopsize, samplerate)

        else:

            frames = encode(frames, framesize, hopsize, samplerate)
            frames = shiftpitch(frames, factors)
            frames = decode(frames, framesize, hopsize, samplerate)

        output = istft(frames, framesize, hopsize)

        output.resize(np.shape(input))

        return output
