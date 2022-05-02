import numpy as np


def linear(x, factor):

    if factor == 1:
        return x.copy()

    N = len(x)
    M = round(N * factor)

    y = np.zeros(x.shape, dtype=x.dtype)

    m = np.arange(min(M, N))
    n = m / factor

    n0 = np.floor(n).astype(int)
    n1 = np.ceil(n).astype(int)

    mask = (0 <= n0) & (n0 < N) & (0 <= n1) & (n1 < N)

    m = m[mask]
    n = n[mask]
    n0 = n0[mask]
    n1 = n1[mask]

    y0 = x[n0]
    y1 = x[n1]

    a = n - n0
    b = n1 - n0

    mask = (n0 == n1)

    a[mask] = 1
    b[mask] = 2

    i = a / b

    y[m] = y0 * (1 - i) + y1 * i

    return y


def bilinear(x0, x1, factor):

    y0 = linear(x0, factor)
    y1 = linear(x1, factor)

    return (y0 + y1) / 2
