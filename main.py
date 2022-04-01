from IO import read, write
from Resampler import linear as resample
from STFT import stft, istft
from Vocoder import encode, decode

import click
import numpy as np


@click.command(context_settings=dict(help_option_names=['-h', '--help']))
@click.option('-i', '--input', required=True, help='input .wav file name')
@click.option('-o', '--output', required=True, help='output .wav file name')
@click.option('-p', '--pitch', default=1.0, show_default=True, help='pitch shifting factor')
@click.option('-w', '--window', default=1024, show_default=True, help='sfft window size')
@click.option('-v', '--overlap', default=32, show_default=True, help='stft window overlap')
def main(input, output, pitch, window, overlap):

    x, sr = read(input)

    factor = pitch
    framesize = window
    hopsize = window // overlap

    frames = stft(x, framesize, hopsize)
    frames = encode(frames, framesize, hopsize, sr)

    for i in range(len(frames)):

        envelope = np.real(frames[i])
        frequencies = np.imag(frames[i])

        envelope = resample(envelope, factor)
        frequencies = resample(frequencies, factor)

        frames[i] = envelope + 1j * frequencies * factor

    frames = decode(frames, framesize, hopsize, sr)

    y = istft(frames, framesize, hopsize)

    write(output, y, sr)


if __name__ == '__main__':

    main()
