from numpy.lib.stride_tricks import sliding_window_view

import numpy as np


def stft(x, framesize, hopsize, wola=True):

    frames = sliding_window_view(x, framesize, writeable=False)[::hopsize]

    M, N = frames.shape

    data = np.zeros((M, N//2+1), complex)

    if wola:
        w = np.hanning(framesize + 1)[:-1]
    else:
        w = np.hanning(framesize)

    for i, frame in enumerate(frames):

        data[i] = np.fft.rfft(w * frame, norm='forward')

    return data


def istft(frames, framesize, hopsize, wola=True):

    M, N = frames.shape

    y = np.zeros((M * hopsize + framesize), float)

    data = sliding_window_view(y, framesize, writeable=True)[::hopsize]

    if wola:
        w = np.hanning(framesize + 1)[:-1]
        w *= hopsize / np.sum(w**2)
    else:
        w = np.hanning(framesize)

    for i, frame in enumerate(frames):

        data[i] += w * np.fft.irfft(frame, norm='forward')

    return y
