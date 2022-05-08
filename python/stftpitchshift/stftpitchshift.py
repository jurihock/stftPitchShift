from stftpitchshift.cepster import lifter
from stftpitchshift.normalizer import normalize
from stftpitchshift.pitcher import shiftpitch
from stftpitchshift.stft import stft, istft, spectrogram
from stftpitchshift.vocoder import encode, decode

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

    def shiftpitch(self, input, factors = 1, quefrency = 0, normalization = False):
        '''
        :param input: The input signal.
        :param factors: The fractional pitch shifting factors.
        :param quefrency: The optional formant lifter quefrency in seconds.
        :param normalization Optionally enable spectral rms normalization.
        :return: The output signal of the equal size.
        '''

        def isnotnormal(x):
            return (np.isinf(x)) | \
                   (np.isnan(x)) | \
                   (abs(x) < np.finfo(x.dtype).tiny)

        framesize = self.framesize
        hopsize = self.hopsize
        samplerate = self.samplerate

        factors = np.asarray(factors).flatten()
        quefrency = int(quefrency * samplerate)

        frames = stft(input, framesize, hopsize)
        frames = encode(frames, framesize, hopsize, samplerate)

        if normalization:

            frames0 = frames.copy()

        if quefrency:

            envelopes = lifter(frames, quefrency)

            mask = isnotnormal(envelopes)

            frames.real /= envelopes
            frames.real[mask] = 0

            frames = shiftpitch(frames, factors)

            frames.real *= envelopes
            frames.real[mask] = 0

        else:

            frames = shiftpitch(frames, factors)

        if normalization:

            frames = normalize(frames, frames0)

        frames = decode(frames, framesize, hopsize, samplerate)

        output = istft(frames, framesize, hopsize)
        output.resize(np.shape(input))

        return output
