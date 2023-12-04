import os, sys
src = os.path.join(os.path.dirname(__file__), '..', 'python')
sys.path.insert(0, src)

from stftpitchshift.io import read, write
from stftpitchshift.resampler import linear as resample
from stftpitchshift.stft import stft
from stftpitchshift.vocoder import encode, decode

from mpl_toolkits.axes_grid1 import make_axes_locatable

import matplotlib.pyplot as plot
import numpy as np


def phase(t, f):

    dt = np.diff(t, prepend=0)
    dp = 2 * np.pi * f * dt
    return np.cumsum(dp)


def chirp(s, lo, hi):

    sr = 44100
    n = int(s * sr)
    t = np.arange(n) / sr
    f = np.linspace(lo, hi, n)
    x = np.sin(phase(t, f))

    return x, sr


def shiftpitch(magn, freq, factor):

    magnT, freqT = np.zeros_like(magn), np.zeros_like(freq)

    for i in range(min(len(magn), len(freq))):

        magnT[i] = resample(magn[i], factor)
        freqT[i] = resample(freq[i], factor) * factor

    return magnT, freqT


def analyze(samples):

    framesize = 1*1024
    hopsize   = framesize // 4

    frames = stft(samples, framesize, hopsize)
    frames = encode(frames, framesize, hopsize, samplerate)

    magn = np.real(frames)
    freq = np.imag(frames)

    with np.errstate(all='ignore'):
        # magn = 20 * np.log10(magn)
        # magn /= np.max(magn)
        # freq /= np.linspace(0, samplerate / 2, freq.shape[-1])
        # freq -= 1
        # freq = np.abs(freq) >= 0.5
        pass

    return magn, freq


if __name__ == '__main__':

    x, samplerate = chirp(5, 100, 1000)
    y, samplerate = chirp(5, 200, 2000)

    magn_x, freq_x = analyze(x)
    magn_y, freq_y = analyze(y)
    magn_z, freq_z = shiftpitch(magn_x, freq_x, 2)

    if False: # error y z

        figure, axes = plot.subplots(1, 2, squeeze=False, sharex=True, sharey=True)

        for i, data in enumerate([magn_y / magn_z - 1, freq_y / freq_z - 1]):

            row   = i // 2
            col   = i % 2

            cmap  = ['inferno']*0 + ['twilight']*2
            title = ['magn', 'freq']

            roi   = (0, len(x) / samplerate, 0, samplerate / 2)

            args  = dict(aspect='auto',
                         cmap=cmap[col],
                         extent=roi,
                         interpolation='nearest',
                         origin='lower')

            data = np.clip(data, -1, +1)

            img = axes[row][col].imshow(data.T, **args)

            div = make_axes_locatable(axes[row][col])
            cax = div.append_axes('right', size='5%', pad=50e-3)
            figure.colorbar(img, cax=cax, orientation='vertical')

            axes[row][col].set_title(f'{title[col]}')
            axes[row][col].set_xlabel('s')
            axes[row][col].set_ylabel('Hz')

        figure.tight_layout()

    if True: # plot x y

        figure, axes = plot.subplots(2, 2, squeeze=False, sharex=True, sharey=True)

        for i, data in enumerate([magn_x, freq_x, magn_y, freq_y]):

            row   = i // 2
            col   = i % 2

            cmap  = ['inferno']*2 + ['twilight']*0
            title = ['magn', 'freq']

            roi   = (0, len(x) / samplerate, 0, samplerate / 2)

            args  = dict(aspect='auto',
                         cmap=cmap[col],
                         extent=roi,
                         interpolation='nearest',
                         origin='lower')

            img = axes[row][col].imshow(data.T, **args)

            div = make_axes_locatable(axes[row][col])
            cax = div.append_axes('right', size='5%', pad=50e-3)
            figure.colorbar(img, cax=cax, orientation='vertical')

            axes[row][col].set_title(f'{title[col]} {row+1}')
            axes[row][col].set_xlabel('s')
            axes[row][col].set_ylabel('Hz')

        figure.tight_layout()

    plot.show()
