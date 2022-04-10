# stftPitchShift

This is a reimplementation of the Stephan M. Bernsee [smbPitchShift.cpp](https://blogs.zynaptiq.com/bernsee/download), a pitch shifting algorithm using the Short-Time Fourier Transform ([STFT](https://www.audiolabs-erlangen.de/resources/MIR/FMP/C2/C2_STFT-Basic.html)).

This repository features two analogical algorithm implementations, [C++](cpp/StftPitchShift) and [Python](python/stftpitchshift). Both contain several [function blocks](#modules) of the same name (but different file extension, of course).

In addition to the base algorithm implementation, it also features spectral [multi pitch shifting](#pitch-shifting) and cepstral [formant preservation](#formant-preservation) extensions.

Both sources contain a ready-to-use [command line tool](#usage) as well as a library for custom needs. See more details in the [build](#build) section.

## Modules

### Vocoder

The *Vocoder* module transforms the DFT spectral data according to the original algorithm, which is actually the *instantaneous frequency estimation* technique. See also [further reading](#further-reading) for more details.

The particular `encode` function replaces the input DFT values by the `magnitude + j * frequency` complex numbers, representing the phase error based frequency estimation in the imaginary part.

The `decode` function does an inverse transformation back to the original DFT complex numbers, by replacing eventually modified frequency value by the reconstructed phase value.

### Pitcher

The *Pitcher* module performs single or multi pitch shifting of the encoded DFT frame depending on the specified fractional factors.

### Resampler

The *Resampler* module provides `linear` and `bilinear` interpolation routines, to actually perform pitch shifting, based on the *Vocoder* DFT transform.

### Cepstrum

The *Cepstrum* module estimates a spectral envelope of the DFT magnitude vector, representing the vocal tract resonances. This computation takes place in the cepstral domain by applying a low-pass filter. The cutoff value of the low-pass filter or *lifter* is the *quefrency* value to be specified in seconds or milliseconds.

### STFT

As the name of this module already implies, it performs the comprehensive *STFT* analysis and synthesis steps.

### IO

The *IO* module provides a simple possibility to read and write `.wav` audio files.

Currently only mono `.wav` files are supported. Please use e.g. [Audacity](http://www.audacityteam.org) or [SoX](http://sox.sourceforge.net) to prepare your audio files for pitch shifting.

## Pitch shifting

### Single pitch

Since the *Vocoder* module transforms the original DFT complex values `real + j * imag` into `magnitude + j * frequency` representation, the single pitch shifting is a comparatively easy task. Both `magnitude` and `frequency` vectors are to be resampled according to the desired pitch shifting factor:

* The factor `1` means no change.
* The factor `<1` means downsampling.
* The factor `>1` means upsampling.

Any fractional resampling factor such as `0.5` requires interpolation. In the simplest case, linear interpolation will be sufficient. Otherwise, bilinear interpolation can also be applied to smooth values between two consecutive STFT hops.

Due to frequency vector alteration, the resampled frequency values needs also be multiplied by the resampling factor.

### Multi pitch

In terms of multi pitch shifting, multiple differently resampled `magnitude` and `frequency` vectors are to be combined together. For example, the magnitude vectors can easily be averaged. But what about the frequency vectors?

The basic concept of this algorithm extension is to only keep the frequency value of the strongest magnitude value. Since the *strongest* magnitude will mask the *weakest* one. Thus, all remaining *masked* frequency values would be *inaudible* and can therefore be omitted.

In this way, the multi pitch shifting can be performed *simultaneously* in the same DFT frame. There is no need to build a separate STFT pipeline for different pitch variations to superimpose the synthesized signals in the time domain.

## Formant preservation

Will soon appear...

## Build

### C++

Use [CMake](http://cmake.org) to build the C++ program and library like so:

```cmd
mkdir build
cd build
cmake ..
cmake --build .
```

To include this library in your C++ audio project, check the [LibStftPitchShift.cmake](cpp/StftPitchShift/LibStftPitchShift.cmake) file and the following minimal example:

```cpp
#include <StftPitchShift/StftPitchShift.h>

StftPitchShift pitchshifter(1024, 256, 44100);

std::vector<float> x(44100);
std::vector<float> y(x.size());

pitchshifter.shiftpitch(x, y, 1);
```

### Python

The Python program `stftpitchshift` can be installed via `pip install stftpitchshift`.

Also feel free to explore the Python class `StftPitchShift` in your personal audio project:

```python
from stftpitchshift import StftPitchShift

pitchshifter = StftPitchShift(1024, 256, 44100)

x = [0] * 44100
y = pitchshifter.shiftpitch(x, 1)
```

## Usage

Both programs C++ and Python provides a similar set of command line arguments:

```
-h  --help     print this help
    --version  print version number

-i  --input    input .wav file name
-o  --output   output .wav file name

-p  --pitch    fractional pitch shifting factors separated by comma
               (default 1.0)

-f  --formant  optional formant lifter quefrency in milliseconds
               (default 0.0)

-w  --window   sfft window size
               (default 1024)

-v  --overlap  stft window overlap
               (default 32)

-d  --debug    plot spectrograms before and after processing
               (only available in the Python version)

    --smb      enable original smb algorithm
               (only available in the C++ version)
```

To apply multiple pitch shifts at once, separate each factor by a comma, e.g. `-p 0.5,1,2`.

To enable the formant preservation feature specify a suitable *quefrency* value in milliseconds. Depending on the source signal, begin with a small value like `-f 1`. Generally, the *quefrency* value has to be smaller than the fundamental period, as reciprocal of the fundamental frequency, of the source signal.

At the moment the formant preservation doesn't seem to work well along with the multi pitch shifting and smaller pitch shifting factors. Further investigation is therefore necessary...

## Further reading

### Instantaneous frequency estimation

* [Fundamentals of Music Processing](http://www.music-processing.de) by Meinard Müller (section 8.2.1 in the second edition or [online](https://www.audiolabs-erlangen.de/resources/MIR/FMP/C8/C8S2_InstantFreqEstimation.html))
* [Digital Audio Effects](http://www.dafx.de) by Udo Zölzer (sections 7.3.1 and 7.3.5 in the second edition)

### Cepstrum analysis and formant changing

* [Digital Audio Effects](http://www.dafx.de) by Udo Zölzer (sections 8.2.3 and 8.3.2 in the second edition)
* [Discrete-Time Signal Processing](https://www.pearson.com/us/higher-education/program/Oppenheim-Discrete-Time-Signal-Processing-3rd-Edition/PGM212808.html) by Oppenheim & Schafer (chapter 13 in the third edition)

## Credits

* [anyoption](https://github.com/hackorama/AnyOption) by Kishan Thomas
* [dr_libs](https://github.com/mackron/dr_libs) by David Reid
* [pocketfft](https://gitlab.mpcdf.mpg.de/mtr/pocketfft) by Martin Reinecke
* [smbPitchShift.cpp](http://blogs.zynaptiq.com/bernsee/download) by Stephan M. Bernsee
