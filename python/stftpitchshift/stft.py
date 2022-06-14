from numpy.lib.stride_tricks import sliding_window_view

import numpy as np


def stft(x, framesize, hopsize):

    frames = sliding_window_view(x, framesize, writeable=False)[::hopsize]

    M, N = frames.shape

    data = np.zeros((M, N//2+1), complex)

    w = 0.5 - 0.5 * np.cos(2 * np.pi * np.arange(framesize) / framesize)

    for i, frame in enumerate(frames):

        data[i] = np.fft.rfft(w * frame, norm='forward')

        # keep dc and nyquist as is
        # data[i][0] = data[i][-1] = 0

    return data


def istft(frames, framesize, hopsize):

    M, N = frames.shape

    y = np.zeros((M * hopsize + framesize), float)

    data = sliding_window_view(y, framesize, writeable=True)[::hopsize]

    w = 0.5 - 0.5 * np.cos(2 * np.pi * np.arange(framesize) / framesize)
    w *= hopsize / np.sum(w**2)

    for i, frame in enumerate(frames):

        # zero dc and nyquist
        frame[0] = frame[-1] = 0

        data[i] += w * np.fft.irfft(frame, norm='forward')

    return y
