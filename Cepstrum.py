import numpy as np


def lowpass(cepstrum, quefrency):

    cepstrum[1:quefrency] *= 2
    cepstrum[quefrency+1:] = 0

    return cepstrum


def lifter(frames, quefrency):

    envelopes = np.zeros(frames.shape)

    for i, frame in enumerate(frames):

        with np.errstate(divide='ignore', invalid='ignore'):
            spectrum = np.log10(np.real(frame))

        cepstrum = np.fft.irfft(spectrum, norm='forward')
        cepstrum = lowpass(cepstrum, quefrency)
        cepstrum = np.fft.rfft(cepstrum, norm='forward')

        envelopes[i] = np.power(10, np.real(cepstrum))

    return envelopes
