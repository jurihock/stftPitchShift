import numpy as np


def linear(x, factor):

    if factor == 1:
        return x.copy()

    y = np.zeros(x.shape, dtype=x.dtype)

    n = len(x)
    m = int(n * factor)

    q = n / m

    i = np.arange(min(n, m))
    k = i * q

    j = np.trunc(k).astype(int)
    k = k - j

    ok = (0 <= j) & (j < n - 1)

    # TODO cosine interpolation
    # k = 0.5 - 0.5 * np.cos(k * np.pi)

    y[i[ok]] = k[ok] * x[j[ok] + 1] + (1 - k[ok]) * x[j[ok]]

    return y


def bilinear(x0, x1, factor):

    y0 = linear(x0, factor)
    y1 = linear(x1, factor)

    return (y0 + y1) / 2
