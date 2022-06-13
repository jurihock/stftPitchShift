import numpy as np


def window(x):

    N, M = x.shape

    left = x[:, :-2]
    right = x[:, +2:]
    middle = x[:, +1:-1]

    y = ((middle + middle) - (left + right)) / (M * 4)

    y = np.pad(y, ((0, 0), (1, 1)))

    return y


def sdft(samples, dftsize):

    N = samples.size
    M = dftsize

    n = np.arange(N + 1)[:, None]
    m = np.arange(M)

    twiddles = np.exp(-2j * np.pi * n * m / (M * 2))

    delayline = np.resize(np.pad(samples, (M * 2, 0)), samples.shape)

    deltas = samples - delayline

    dfts = deltas[:, None] * twiddles[:-1]

    for i in range(1, N):

        dfts[i] += dfts[i - 1]

    dfts *= np.conj(twiddles[1:])
    dfts = window(dfts)

    return dfts


def isdft(dfts, latency=1):

    N, M = dfts.shape

    if latency == 1:

        twiddles = np.array([-1 if m % 2 else +1 for m in range(M)])

    else:

        weight = 2 / (1 - np.cos(np.pi * latency))

        twiddles = weight * np.exp(1j * np.pi * latency * np.arange(M))

    samples = np.sum(np.real(dfts * twiddles), axis=-1)

    return samples


if __name__ == '__main__':

    import matplotlib.pyplot as plot

    sr = 44100

    f = 1000
    t = np.arange(1 * sr) / sr
    x = np.sin(2 * np.pi * f * t)

    dftsize = 1024
    latency = 1

    dfts = sdft(x, dftsize)
    samples = isdft(dfts, latency)

    assert x.shape[0] == dfts.shape[0]
    assert x.shape == samples.shape

    print(dfts.shape, samples.shape)

    with np.errstate(divide='ignore', invalid='ignore'):
        db = 20 * np.log10(np.abs(dfts))

    roi = (np.min(t), np.max(t), 0, sr / (dftsize * 2))

    # plot.figure()
    # plot.plot(t, x, alpha=0.5)
    # plot.plot(t, samples, alpha=0.5)

    plot.figure()
    plot.imshow(db.T, extent=roi, cmap='inferno', aspect='auto', interpolation='nearest', origin='lower')
    plot.clim(-120, 0)

    plot.show()
