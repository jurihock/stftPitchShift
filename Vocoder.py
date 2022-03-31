import numpy as np


def resample(x, factor):

    N = len(x)
    M = round(N * factor)

    y = np.interp(
        np.linspace(0, 1, M, endpoint=False),
        np.linspace(0, 1, N, endpoint=False),
        x)

    y.resize(x.shape)

    return y


def wrap(x):

    return (x + np.pi) % (2 * np.pi) - np.pi


def encode(frames, framesize, hopsize, sr):

    M, N = frames.shape

    phaseinc = 2 * np.pi * hopsize / framesize
    freqinc = sr / framesize

    buffer = np.zeros(N)
    data = np.zeros((M, N), complex)

    for m, frame in enumerate(frames):

        abs = np.abs(frame)
        arg = np.angle(frame)

        delta = arg - buffer
        buffer = arg

        i = np.arange(N)
        j = wrap(delta - i * phaseinc) / phaseinc

        freq = (i + j) * freqinc

        data[m] = abs + 1j * freq

    return data


def decode(frames, framesize, hopsize, sr):

    M, N = frames.shape

    phaseinc = 2 * np.pi * hopsize / framesize
    freqinc = sr / framesize

    buffer = np.zeros(N)
    data = np.zeros((M, N), complex)

    for m, frame in enumerate(frames):

        abs = np.real(frame)
        freq = np.imag(frame)

        i = np.arange(N)
        j = (freq - i * freqinc) / freqinc

        delta = (i + j) * phaseinc

        buffer += delta
        arg = buffer

        data[m] = abs * np.exp(1j * arg)

    return data
