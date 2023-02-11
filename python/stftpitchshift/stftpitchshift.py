from stftpitchshift.cepster import lifter
from stftpitchshift.normalizer import normalize
from stftpitchshift.pitcher import shiftpitch
from stftpitchshift.resampler import linear as resample
from stftpitchshift.stft import stft, istft
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

    def shiftpitch(self, input, factors = 1, quefrency = 0, distortion = 1, normalization = False):
        '''
        :param input: The input signal.
        :param factors: The fractional pitch shifting factors.
        :param quefrency: The optional formant lifter quefrency in seconds.
        :param distortion: The fractional timbre shifting factor.
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

            if distortion != 1:

                envelopes[mask] = 0

                for i in range(len(envelopes)):

                    envelopes[i] = resample(envelopes[i], distortion)

                mask = isnotnormal(envelopes)

            frames = shiftpitch(frames, factors, samplerate)

            frames.real *= envelopes
            frames.real[mask] = 0

        else:

            frames = shiftpitch(frames, factors, samplerate)

        if normalization:

            frames = normalize(frames, frames0)

        frames = decode(frames, framesize, hopsize, samplerate)

        output = istft(frames, framesize, hopsize)

        # disable reference count check on resize,
        # since the output variable owns the data
        # returned by istft (see also issue #31)
        output.resize(np.shape(input), refcheck=False)
        assert np.shape(input) == np.shape(output)

        return output
