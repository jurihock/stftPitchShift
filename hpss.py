import click
import matplotlib.pyplot as pyplot
import numpy as np
import os
import sys

python = os.path.join(os.path.dirname(__file__), 'python')

sys.path.insert(0, python)

@click.command('hpss', help='Harmonic percussive source separation', no_args_is_help=True, context_settings=dict(help_option_names=['-h', '--help']))
@click.argument('input')
@click.option('-w', '--window', default=1024, show_default=True, help='stft window size')
@click.option('-v', '--overlap', default=32, show_default=True, help='stft window overlap')
@click.option('-k', '--kernel', default=25, show_default=True, help='median filter kernel size')
def hpss(input, window, overlap, kernel):

    from scipy.signal import medfilt2d
    from stftpitchshift.io import read, write
    from stftpitchshift.stft import stft, istft

    framesize = window
    hopsize = window // overlap

    samples, sr = read(input)

    frames = stft(samples, framesize, hopsize)

    magnitude = np.abs(frames)

    median1 = medfilt2d(magnitude, (kernel, 1))
    median2 = medfilt2d(magnitude, (1, kernel))

    mask1 = (median1 >= median2).astype(float)
    mask2 = (median2 >= median1).astype(float)

    # mask1 = median1**2 / (median1**2 + median2**2)
    # mask2 = median2**2 / (median1**2 + median2**2)

    frames1 = frames * mask1
    frames2 = frames * mask2

    y1 = istft(frames1, framesize, hopsize)
    y2 = istft(frames2, framesize, hopsize)

    output = os.path.splitext(input)

    write(f'{output[0]}.h{output[1]}', y1, sr)
    write(f'{output[0]}.p{output[1]}', y2, sr)

if __name__ == '__main__':

    hpss()
