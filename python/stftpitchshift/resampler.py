import numpy as np


def linear(x, factor):

    if factor == 1:
        return x.copy()

    N = len(x)
    M = round(N * factor)

    y = np.zeros(x.shape, dtype=x.dtype)

    for m in range(min(M, N)):
    
        n = m / factor

        n0 = int(np.floor(n))
        n1 = int(np.ceil(n))
        
        if n0 < 0 or N <= n0:
            continue

        if n1 < 0 or N <= n1:
            continue

        if n0 == n1:
            y[m] = x[n0]
            continue
        
        y0 = x[n0]
        y1 = x[n1]

        i = (n - n0) / (n1 - n0)
        
        y[m] = y0 * (1 - i) + y1 * i

    return y


def bilinear(x0, x1, factor):

    y0 = linear(x0, factor)
    y1 = linear(x1, factor)

    return (y0 + y1) / 2
