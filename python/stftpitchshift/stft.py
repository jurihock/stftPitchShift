from numpy.lib.stride_tricks import sliding_window_view

import numpy as np


def stft(x, framesize, hopsize):

    # return stft_orig(x, framesize, hopsize)
    return stft_tfr(x, framesize, hopsize)


def stft_tfr(x, framesize, hopsize):

    import librosa

    x = np.atleast_1d(x)
    assert x.ndim == 1

    analysis_window_size = np.ravel(framesize)[0]
    synthesis_window_size = np.ravel(framesize)[-1]

    dftsize = analysis_window_size

    freq, time, magn = librosa.reassigned_spectrogram(
        x,
        sr=44100,
        n_fft=analysis_window_size,
        hop_length=hopsize,
        window='hann',
        center=False,
        reassign_frequencies=True,
        reassign_times=True,
        ref_power=1e-9,
        fill_nan=False,
        clip=True)

    freq = freq.T
    time = time.T
    magn = magn.T

    freq[np.isnan(freq)] = 0
    time[np.isnan(time)] = 0
    magn[np.isnan(magn)] = 0

    magn /= analysis_window_size

    return magn + 1j * freq


def stft_orig(x, framesize, hopsize):

    # check input

    x = np.atleast_1d(x)

    assert x.ndim == 1, \
        f'Expected 1D array (samples,),' + \
        f' got {x.shape}!'

    # construct window

    analysis_window_size = np.ravel(framesize)[0]
    synthesis_window_size = np.ravel(framesize)[-1]

    assert analysis_window_size >= synthesis_window_size, \
        f'Analysis window ({analysis_window_size}) must be greater' + \
        f' or equal to synthesis window ({synthesis_window_size})!'

    W = asymmetric_analysis_window(analysis_window_size, synthesis_window_size) \
        if analysis_window_size != synthesis_window_size else \
        symmetric_window(analysis_window_size)

    # perform analysis

    frames0 = sliding_window_view(x, analysis_window_size, writeable=False)[::hopsize]
    frames1 = np.fft.rfft(frames0 * W, axis=-1, norm='forward')

    if False: # optionally zero dc and nyquist
        frames1[:,  0] = 0
        frames1[:, -1] = 0

    return frames1


def istft(frames, framesize, hopsize):

    # check input

    frames = np.atleast_2d(frames)

    assert frames.ndim == 2, \
        f'Expected 2D array (samples,frequencies),' + \
        f' got {frames.shape}!'

    # construct window

    analysis_window_size = np.ravel(framesize)[0]
    synthesis_window_size = np.ravel(framesize)[-1]

    assert analysis_window_size >= synthesis_window_size, \
        f'Analysis window ({analysis_window_size}) must be greater' + \
        f' or equal to synthesis window ({synthesis_window_size})!'

    A = asymmetric_analysis_window(analysis_window_size, synthesis_window_size) \
        if analysis_window_size != synthesis_window_size else \
        symmetric_window(analysis_window_size)

    S = asymmetric_synthesis_window(analysis_window_size, synthesis_window_size) \
        if analysis_window_size != synthesis_window_size else \
        symmetric_window(synthesis_window_size)

    W = S * hopsize / np.sum(A * S)

    # perform synthesis

    N = frames.shape[0] * hopsize + analysis_window_size

    y = np.zeros((N), float)

    if True: # optionally zero dc and nyquist
        frames[:,  0] = 0
        frames[:, -1] = 0

    frames0 = sliding_window_view(y, analysis_window_size, writeable=True)[::hopsize]
    frames1 = np.fft.irfft(frames, axis=-1, norm='forward') * W

    for i in range(min(len(frames0), len(frames1))):
        frames0[i] += frames1[i]

    return y


def symmetric_window(symmetric_window_size):

    n = symmetric_window_size

    window = 0.5 - 0.5 * np.cos(2 * np.pi * np.arange(n) / n)

    return window


def asymmetric_analysis_window(analysis_window_size, synthesis_window_size):

    n = analysis_window_size
    m = synthesis_window_size // 2

    left = symmetric_window(2 * n - 2 * m)
    right = symmetric_window(2 * m)

    window = np.zeros(n)

    window[:n-m] = left[:n-m]
    window[-m:] = right[-m:]

    return window


def asymmetric_synthesis_window(analysis_window_size, synthesis_window_size):

    n = analysis_window_size
    m = synthesis_window_size // 2

    left = symmetric_window(2 * n - 2 * m)
    right = symmetric_window(2 * m)

    window = np.zeros(n)

    window[n-m-m:n-m] = np.square(right[:m]) / left[n-m-m:n-m]
    window[-m:] = right[-m:]

    return window
