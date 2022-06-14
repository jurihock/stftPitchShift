import matplotlib
import matplotlib.pyplot as plot
import numpy as np


def spectrogram(frames, framesize, hopsize, samplerate, xlim=None, ylim=None, clim=-120, cmap='inferno'):

    def lim():

        if xlim is not None:
            if isinstance(xlim, (list, tuple)):
                plot.xlim(xlim)
            else:
                plot.xlim(0, xlim)

        if ylim is not None:
            if isinstance(ylim, (list, tuple)):
                plot.ylim(ylim)
            else:
                plot.ylim(0, ylim)

        if clim is not None:
            if isinstance(clim, (list, tuple)):
                plot.clim(clim)
            else:
                plot.clim(clim, 0)

    with np.errstate(divide='ignore', invalid='ignore'):
        data = 20 * np.log10(np.abs(frames))

    timestamps = np.arange(frames.shape[0]) * hopsize / samplerate
    frequencies = np.arange(frames.shape[1]) * samplerate / framesize

    roi = (np.min(timestamps), np.max(timestamps), np.min(frequencies), np.max(frequencies))

    colormap = matplotlib.cm.get_cmap(cmap)
    colormap.set_bad(colormap(0))

    plot.imshow(data.T, extent=roi, cmap=colormap, aspect='auto', interpolation='nearest', origin='lower')

    plot.xlabel('s')
    plot.ylabel('Hz')

    colorbar = plot.colorbar()
    colorbar.set_label('dB')

    lim()

    return plot


def phasogram(frames, framesize, hopsize, samplerate, xlim=None, ylim=None, clim=None, cmap='twilight'):

    def lim():

        if xlim is not None:
            if isinstance(xlim, (list, tuple)):
                plot.xlim(xlim)
            else:
                plot.xlim(0, xlim)

        if ylim is not None:
            if isinstance(ylim, (list, tuple)):
                plot.ylim(ylim)
            else:
                plot.ylim(0, ylim)

        if clim is not None:
            if isinstance(clim, (list, tuple)):
                plot.clim(clim)
            else:
                plot.clim(-clim, +clim)
        else:
            plot.clim(-np.pi, +np.pi)

    data = np.angle(frames)

    timestamps = np.arange(frames.shape[0]) * hopsize / samplerate
    frequencies = np.arange(frames.shape[1]) * samplerate / framesize

    roi = (np.min(timestamps), np.max(timestamps), np.min(frequencies), np.max(frequencies))

    plot.imshow(data.T, extent=roi, cmap=cmap, aspect='auto', interpolation='nearest', origin='lower')

    plot.xlabel('s')
    plot.ylabel('Hz')

    colorbar = plot.colorbar()
    colorbar.set_label('rad')

    lim()

    return plot
