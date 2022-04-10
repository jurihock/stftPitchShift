from StftPitchShift.Cepstrum import lifter
from StftPitchShift.Pitcher import shiftpitch
from StftPitchShift.STFT import stft, istft, spectrogram
from StftPitchShift.Vocoder import encode, decode

import numpy as np


class StftPitchShift:
    '''
    Short-time Fourier transform (STFT) based pitch shifting.
    '''

    def __init__(self, framesize, hopsize, samplerate):
        '''
        :param framesize: The STFT frame size in samples.
        :param hopsize: The STFT hop size in samples.
        :param samplerate: The sample rate of the signal in hertz.
        '''

        self.framesize = framesize
        self.hopsize = hopsize
        self.samplerate = samplerate

    def shiftpitch(self, input, factors = 1, quefrency = 0):
        '''
        :param input: The input signal.
        :param factors: The fractional pitch shifting factors.
        :param quefrency: The optional formant lifter quefrency in seconds.
        :return: The output signal of the equal size.
        '''

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
