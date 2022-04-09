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

def bilinear(x0, x1, factor):

    y0 = linear(x0, factor)
    y1 = linear(x1, factor)

    return (y0 + y1) / 2
