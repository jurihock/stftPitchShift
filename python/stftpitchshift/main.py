from stftpitchshift import StftPitchShift
from stftpitchshift import __version__ as version
from stftpitchshift.io import read, write
from stftpitchshift.stft import stft, istft, spectrogram

import click
import matplotlib.pyplot as plot


@click.command(context_settings=dict(help_option_names=['-h', '--help']))
@click.version_option(version=version, message='%(version)s')
@click.option('-i', '--input', required=True, help='input .wav file name')
@click.option('-o', '--output', required=True, help='output .wav file name')
@click.option('-p', '--pitch', default='1.0', show_default=True, help='fractional pitch shifting factors separated by comma')
@click.option('-f', '--formant', default='0.0', show_default=True, help='optional formant lifter quefrency in milliseconds')
@click.option('-w', '--window', default=1024, show_default=True, help='sfft window size')
@click.option('-v', '--overlap', default=32, show_default=True, help='stft window overlap')
@click.option('-d', '--debug', is_flag=True, default=False, help='plot spectrograms before and after processing')
def main(input, output, pitch, formant, window, overlap, debug):

    x, samplerate = read(input)

    factors = [float(factor) for factor in pitch.split(',')]
    quefrency = float(formant) * 1e-3

    framesize = window
    hopsize = window // overlap

    pitchshifter = StftPitchShift(framesize, hopsize, samplerate)

    y = pitchshifter.shiftpitch(x, factors, quefrency)

    write(output, y, samplerate)

    if debug:

        framesX = stft(x, framesize, hopsize)
        framesY = stft(y, framesize, hopsize)

        figure = plot.figure()

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
