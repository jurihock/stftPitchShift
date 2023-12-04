import numpy as np


def wrap(x):

    return (x + np.pi) % (2 * np.pi) - np.pi


def encode(frames, framesize, hopsize, samplerate):

    # return encode_orig(frames, framesize, hopsize, samplerate)
    return encode_tfr(frames, framesize, hopsize, samplerate)


def encode_tfr(frames, framesize, hopsize, samplerate):

    return frames


def encode_orig(frames, framesize, hopsize, samplerate):

    M, N = frames.shape

    analysis_framesize = np.ravel(framesize)[0]
    synthesis_framesize = np.ravel(framesize)[-1]

    freqinc = samplerate / analysis_framesize
    phaseinc = 2 * np.pi * hopsize / analysis_framesize

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


def decode(frames, framesize, hopsize, samplerate):

    M, N = frames.shape

    analysis_framesize = np.ravel(framesize)[0]
    synthesis_framesize = np.ravel(framesize)[-1]

    freqinc = samplerate / analysis_framesize
    phaseinc = 2 * np.pi * hopsize / analysis_framesize

    # compensate asymmetric synthesis window by virtual time shifting #38
    timeshift = 2 * np.pi * synthesis_framesize * np.arange(N) / N \
        if synthesis_framesize != analysis_framesize else 0

    buffer = np.zeros(N)
    data = np.zeros((M, N), complex)

    for m, frame in enumerate(frames):

        abs = np.real(frame)
        freq = np.imag(frame)

        i = np.arange(N)
        j = (freq - i * freqinc) / freqinc

        delta = (i + j) * phaseinc

        buffer += delta
        arg = buffer.copy()

        arg -= timeshift #38

        data[m] = abs * np.exp(1j * arg)

    return data
