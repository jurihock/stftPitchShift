from stftpitchshift import StftPitchShift
from stftpitchshift import __version__ as version
from stftpitchshift.io import read, write
from stftpitchshift.stft import stft, istft, spectrogram

import click
import matplotlib.pyplot as plot
import numpy as np
import re


@click.command('stftpitchshift', help='STFT based multi pitch shifting with optional formant preservation', no_args_is_help=True, context_settings=dict(help_option_names=['-h', '--help']))
@click.version_option(version=version, message='%(version)s')
@click.option('-i', '--input', required=True, help='input .wav file name')
@click.option('-o', '--output', required=True, help='output .wav file name')
@click.option('-p', '--pitch', default='1.0', show_default=True, help='fractional pitch shifting factors separated by comma')
@click.option('-q', '--quefrency', default='0.0', show_default=True, help='optional formant lifter quefrency in milliseconds')
@click.option('-r', '--rms', is_flag=True, default=False, help='enable spectral rms normalization')
@click.option('-w', '--window', default=1024, show_default=True, help='sfft window size')
@click.option('-v', '--overlap', default=32, show_default=True, help='stft window overlap')
@click.option('-d', '--debug', is_flag=True, default=False, help='plot spectrograms before and after processing')
def main(input, output, pitch, quefrency, rms, window, overlap, debug):

    def parse(value): return value.startswith('+') or value.startswith('-') or (value.startswith('0') and '.' not in value)
    def semitone(value): return pow(2, float(re.match('([+,-]?\\d+){1}([+,-]\\d+){0,1}', value)[1]) / 12)
    def cent(value): return pow(2, float(re.match('([+,-]?\\d+){1}([+,-]\\d+){0,1}', value)[2] or 0) / 1200)

    x, samplerate = read(input)

    factors = list(set(semitone(factor) * cent(factor) if parse(factor) else float(factor) for factor in pitch.split(',')))
    quefrency = float(quefrency) * 1e-3
    normalization = rms

    framesize = window
    hopsize = window // overlap

    pitchshifter = StftPitchShift(framesize, hopsize, samplerate)

    channels = x.shape[-1] if x.ndim > 1 else 1

    x = x[:, None] if channels == 1 else x

    y = np.stack([
        pitchshifter.shiftpitch(x[:, channel], factors, quefrency, normalization)
        for channel in range(channels)
    ], axis=-1)

    write(output, y, samplerate)

    if debug:

        for channel in range(channels):

            framesX = stft(x[:, channel], framesize, hopsize)
            framesY = stft(y[:, channel], framesize, hopsize)

            figure = plot.figure(f'Channel {channel+1}/{channels}')

            spectrogramX = figure.add_subplot(2, 1, 1, title='Input Spectrogram')
            spectrogram(framesX, framesize, hopsize, samplerate)

            spectrogramY = figure.add_subplot(2, 1, 2, title='Output Spectrogram')
            spectrogram(framesY, framesize, hopsize, samplerate)

            spectrogramX.get_shared_x_axes().join(spectrogramX, spectrogramY)
            spectrogramX.get_shared_y_axes().join(spectrogramX, spectrogramY)

            plot.tight_layout()

        plot.show()


if __name__ == '__main__':

    main()
