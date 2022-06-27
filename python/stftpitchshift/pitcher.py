from stftpitchshift.resampler import linear as resample

import numpy as np


def shiftpitch(frames, factors, samplerate):

    for i in range(len(frames)):

        magnitudes = np.real(frames[i])
        frequencies = np.imag(frames[i])

        magnitudes = np.vstack([resample(magnitudes, factor) for factor in factors])
        frequencies = np.vstack([resample(frequencies, factor) * factor for factor in factors])

        magnitudes[(frequencies <= 0) | (frequencies >= samplerate / 2)] = 0

        mask = np.argmax(magnitudes, axis=0)

        magnitudes = np.take_along_axis(magnitudes, mask[None,:], axis=0)
        frequencies = np.take_along_axis(frequencies, mask[None,:], axis=0)

        frames[i] = magnitudes + 1j * frequencies

    return frames
