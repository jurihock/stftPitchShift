from numpy.lib.stride_tricks import sliding_window_view

import numpy as np


def stft(x, framesize, hopsize, dftsize=None):

    # check input

    x = np.atleast_1d(x)

    assert x.ndim == 1, f'Expected 1D array (samples,), got {x.shape}!'

    # construct window

    analysis_window_size = framesize if dftsize is None else \
                           np.fft.irfft([0] * dftsize).size

    synthesis_window_size = framesize

    assert analysis_window_size >= synthesis_window_size, \
        f'Invalid framesize and dftsize combination!'

    W = asymmetric_analysis_window(analysis_window_size, synthesis_window_size) \
        if analysis_window_size != synthesis_window_size else \
        symmetric_window(analysis_window_size)

    # perform analysis

    frames0 = sliding_window_view(x, analysis_window_size, writeable=False)[::hopsize]
    frames1 = np.fft.rfft(frames0 * W, axis=-1, norm='forward')

    # optionally zero dc and nyquist
    if False:
        frames1[:,  0] = 0
        frames1[:, -1] = 0

    return frames1


def istft(frames, framesize, hopsize, dftsize=None):

    # check input

    frames = np.atleast_2d(frames)

    assert frames.ndim == 2, f'Expected 2D array (samples,frequencies), got {frames.shape}!'

    # construct window

    analysis_window_size = framesize if dftsize is None else \
                           np.fft.irfft([0] * dftsize).size

    synthesis_window_size = framesize

    assert analysis_window_size >= synthesis_window_size, \
        f'Invalid framesize and dftsize combination!'

    A = asymmetric_analysis_window(analysis_window_size, synthesis_window_size) \
        if analysis_window_size != synthesis_window_size else \
        symmetric_window(analysis_window_size)

    S = asymmetric_synthesis_window(analysis_window_size, synthesis_window_size) \
        if analysis_window_size != synthesis_window_size else \
        symmetric_window(analysis_window_size)

    W = S * hopsize / np.sum(A * S)

    # perform synthesis

    N = frames.shape[0] * hopsize + analysis_window_size

    y = np.zeros((N), float)

    # optionally zero dc and nyquist
    if True:
        frames[:,  0] = 0
        frames[:, -1] = 0

    frames0 = sliding_window_view(y, analysis_window_size, writeable=True)[::hopsize]
    frames1 = np.fft.irfft(frames, axis=-1, norm='forward') * W

    for i in range(min(len(frames0), len(frames1))):
        frames0[i] += frames1[i]

    return y


def symmetric_window(n):

    return 0.5 - 0.5 * np.cos(2 * np.pi * np.arange(n) / n)


def asymmetric_analysis_window(n, m):

    m //= 2

    left = symmetric_window(2 * n - 2 * m)
    right = symmetric_window(2 * m)

    weights = np.zeros(n)

    weights[:n-m] = left[:n-m]
    weights[-m:] = right[-m:]

    return weights


def asymmetric_synthesis_window(n, m):

    m //= 2

    left = symmetric_window(2 * n - 2 * m)
    right = symmetric_window(2 * m)

    weights = np.zeros(n)

    weights[n-m-m:n-m] = right[:m] / left[n-m-m:n-m]
    weights[-m:] = right[-m:]

    return weights
