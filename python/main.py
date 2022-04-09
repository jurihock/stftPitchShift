from Cepstrum import lifter
from IO import read, write
from Pitcher import shiftpitch
from STFT import stft, istft, spectrogram
from Vocoder import encode, decode

import click
import matplotlib.pyplot as plot
import numpy as np


@click.command(context_settings=dict(help_option_names=['-h', '--help']))
@click.option('-i', '--input', required=True, help='input .wav file name')
@click.option('-o', '--output', required=True, help='output .wav file name')
@click.option('-p', '--pitch', default='1.0', show_default=True, help='fractional pitch shifting factors separated by comma')
@click.option('-f', '--formant', default='0.0', show_default=True, help='optional formant lifter quefrency in milliseconds')
@click.option('-w', '--window', default=1024, show_default=True, help='sfft window size')
@click.option('-v', '--overlap', default=32, show_default=True, help='stft window overlap')
@click.option('-d', '--debug', is_flag=True, default=False, help='plot spectrograms before and after processing')
def main(input, output, pitch, formant, window, overlap, debug):

    x, sr = read(input)

    factors = [float(factor) for factor in pitch.split(',')]
    quefrency = int(float(formant) * 1e-3 * sr)

    framesize = window
    hopsize = window // overlap

    frames = stft(x, framesize, hopsize)

    if quefrency:

        frames0 = frames.copy() if debug else None
        frames = encode(frames, framesize, hopsize, sr)

        envelopes = lifter(frames, quefrency)

        frames.real /= envelopes
        frames = shiftpitch(frames, factors)
        frames.real *= envelopes

        frames = decode(frames, framesize, hopsize, sr)
        frames1 = frames.copy() if debug else None

    else:

        frames0 = frames.copy() if debug else None
        frames = encode(frames, framesize, hopsize, sr)
        frames = shiftpitch(frames, factors)
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
