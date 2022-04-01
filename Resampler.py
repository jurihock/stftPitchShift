import numpy as np


def linear(x, factor):

    N = len(x)
    M = round(N * factor)

    y = np.interp(
        np.linspace(0, 1, M, endpoint=False),
        np.linspace(0, 1, N, endpoint=False),
        x)

    y.resize(x.shape)

    return y
