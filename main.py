from IO import read, write
from Resampler import linear as resample
from STFT import stft, istft, spectrogram
from Vocoder import encode, decode

import click
import matplotlib.pyplot as plot
import numpy as np


@click.command(context_settings=dict(help_option_names=['-h', '--help']))
@click.option('-i', '--input', required=True, help='input .wav file name')
@click.option('-o', '--output', required=True, help='output .wav file name')
@click.option('-p', '--pitch', default=1.0, show_default=True, help='pitch shifting factor')
@click.option('-w', '--window', default=1024, show_default=True, help='sfft window size')
@click.option('-v', '--overlap', default=32, show_default=True, help='stft window overlap')
@click.option('-d', '--debug', is_flag=True, default=False, help='plot spectrograms before and after processing')
def main(input, output, pitch, window, overlap, debug):

    x, sr = read(input)

    factor = pitch
    framesize = window
    hopsize = window // overlap

    frames = stft(x, framesize, hopsize)

    frames0 = frames.copy() if debug else None
    frames = encode(frames, framesize, hopsize, sr)

    for i in range(len(frames)):

        envelope = np.real(frames[i])
        frequencies = np.imag(frames[i])

        envelope = resample(envelope, factor)
        frequencies = resample(frequencies, factor)

        frames[i] = envelope + 1j * frequencies * factor

    frames = decode(frames, framesize, hopsize, sr)
    frames1 = frames.copy() if debug else None

    y = istft(frames, framesize, hopsize)

    write(output, y, sr)

    if debug:

        figure = plot.figure()

        spectrogram0 = figure.add_subplot(2, 1, 1, title='Input Spectrogram')
        spectrogram(frames0, framesize, hopsize, sr)

        spectrogram1 = figure.add_subplot(2, 1, 2, title='Output Spectrogram')
        spectrogram(frames1, framesize, hopsize, sr)

        spectrogram0.get_shared_x_axes().join(spectrogram0, spectrogram1)
        spectrogram0.get_shared_y_axes().join(spectrogram0, spectrogram1)

        plot.tight_layout()
        plot.show()


if __name__ == '__main__':

    main()
