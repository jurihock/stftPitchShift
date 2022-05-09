# stftPitchShift

![language](https://img.shields.io/badge/language-C%2B%2B-blue)
![language](https://img.shields.io/badge/language-Python-blue)
![license](https://img.shields.io/github/license/jurihock/stftPitchShift?color=blue)
![build cpp](https://img.shields.io/github/workflow/status/jurihock/stftPitchShift/cpp?label=build%20cpp)
![build python](https://img.shields.io/github/workflow/status/jurihock/stftPitchShift/python?label=build%20python)
![tag](https://img.shields.io/github/v/tag/jurihock/stftPitchShift?color=gold)
![pypi](https://img.shields.io/pypi/v/stftpitchshift?color=gold)

This is a reimplementation of the Stephan M. Bernsee [smbPitchShift.cpp](https://blogs.zynaptiq.com/bernsee/download), a pitch shifting algorithm using the Short-Time Fourier Transform ([STFT](https://www.audiolabs-erlangen.de/resources/MIR/FMP/C2/C2_STFT-Basic.html)), especially for vocal audio signals.

This repository features two analogical algorithm implementations, [C++](cpp/StftPitchShift) and [Python](python/stftpitchshift). Both contain several [function blocks](#modules) of the same name (but different file extension, of course).

In addition to the base algorithm implementation, it also features spectral [multi pitch shifting](#pitch-shifting) and cepstral [formant preservation](#formant-preservation) extensions.

Both sources contain a ready-to-use [command line tool](#usage) as well as a library for custom needs. See more details in the [build](#build) section.

## Modules

<details>
<summary><strong>StftPitchShift</strong></summary>

The *StftPitchShift* module provides a full-featured audio processing chain to perform the pitch shifting of a single audio track, based on the built in *STFT* implementation.

Exclusively in the C++ environment the additional *StftPitchShiftCore* module can be used to embed this pitch shifting implementation in an existing real-time *STFT* pipeline.
</details>

<details>
<summary><strong>Vocoder</strong></summary>

The *Vocoder* module transforms the DFT spectral data according to the original algorithm, which is actually the *instantaneous frequency estimation* technique. See also [further reading](#further-reading) for more details.

The particular `encode` function replaces the input DFT values by the `magnitude + j * frequency` complex numbers, representing the phase error based frequency estimation in the imaginary part.

The `decode` function does an inverse transformation back to the original DFT complex numbers, by replacing eventually modified frequency value by the reconstructed phase value.
</details>

<details>
<summary><strong>Pitcher</strong></summary>

The *Pitcher* module performs single or multi pitch shifting of the encoded DFT frame depending on the specified fractional factors.
</details>

<details>
<summary><strong>Resampler</strong></summary>

The *Resampler* module provides the `linear` interpolation routine, to actually perform pitch shifting, based on the *Vocoder* DFT transform.
</details>

<details>
<summary><strong>Cepster</strong></summary>

The *Cepster* module estimates a spectral envelope of the DFT magnitude vector, representing the vocal tract resonances. This computation takes place in the cepstral domain by applying a low-pass filter. The cutoff value of the low-pass filter or *lifter* is the *quefrency* value to be specified in seconds or milliseconds.
</details>

<details>
<summary><strong>Normalizer</strong></summary>

The *Normalizer* module optionally performs a [RMS normalization](https://en.wikipedia.org/wiki/Audio_normalization) right after pitch shifting relative to the original signal to get about the same loudness level. This correction takes place in the frequency domain each DFT frame separately.
</details>

<details>
<summary><strong>STFT</strong></summary>

As the name of this module already implies, it performs the comprehensive *STFT* analysis and synthesis steps.
</details>

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

The basic concept of this algorithm extension is to only keep the frequency value of the strongest magnitude value. So the *strongest* magnitude will mask the *weakest* one. Thus, all remaining *masked* components become *inaudible*.

In this way, the multi pitch shifting can be performed *simultaneously* in the same DFT frame. There is no need to build a separate STFT pipeline for different pitch variations to superimpose the synthesized signals in the time domain.

## Formant preservation

The pitch shifting also causes distortion of the original [vocal formants](https://en.wikipedia.org/wiki/Formant), leading to a so called *Mickey Mouse* effect if scaled up. One possibility to reduce this artifact, is to exclude the formant feature from the pitch shifting procedure.

The vocal formants are represented by the *spectral envelope*, which is given by the smoothed DFT mangitude vector. In this implementation, the smoothing of the DFT mangitude vector takes place in the cepstral domain by low-pass *liftering*. The extracted envelope is then removed from the original DFT magnitude. The remaining *residual* or *excitation* signal goes through the pitch shifting algorithm. After that, the previously extracted envelope is combined with the processed residual.

## Build

### C++

Use [CMake](http://cmake.org) to manually build the C++ library, main and example programs like this:

```cmd
cmake -S . -B build
cmake --build build
```

Or alternatively just get the packaged library from:

* Vcpkg repository [stftpitchshift](https://vcpkg.info/port/stftpitchshift) or
* Ubuntu repository [ppa:jurihock/stftpitchshift](https://launchpad.net/~jurihock/+archive/ubuntu/stftpitchshift).

To include this library in your C++ audio project, study the minimal C++ example in the examples folder:

```cpp
#include <StftPitchShift/StftPitchShift.h>

using namespace stftpitchshift;

StftPitchShift pitchshifter(1024, 256, 44100);

std::vector<float> x(44100);
std::vector<float> y(x.size());

pitchshifter.shiftpitch(x, y, 1);
```

Optionally specify following CMake options for custom builds:

* `-DBUILD_SHARED_LIBS=ON` to enable a [shared](https://cmake.org/cmake/help/latest/variable/BUILD_SHARED_LIBS.html) library build,
* `-DVCPKG=ON` to enable the [vcpkg](https://vcpkg.io) compatible library only build without executables,
* `-DDEB=ON` to enable the [deb](https://en.wikipedia.org/wiki/Deb_(file_format)) package build for library and main executable.

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

Both programs C++ and Python provides a similar set of command line options:

```
-h  --help       print this help
    --version    print version number

-i  --input      input .wav file name
-o  --output     output .wav file name

-p  --pitch      fractional pitch shifting factors separated by comma
                 (default 1.0)

-q  --quefrency  optional formant lifter quefrency in milliseconds
                 (default 0.0)

-r  --rms        enable spectral rms normalization

-w  --window     sfft window size
                 (default 1024)

-v  --overlap    stft window overlap
                 (default 32)

-c  --chrono     enable runtime measurements
                 (only available in the C++ version)

-d  --debug      plot spectrograms before and after processing
                 (only available in the Python version)
```

Currently only `.wav` files are supported. Please use e.g. [Audacity](http://www.audacityteam.org) or [SoX](http://sox.sourceforge.net) to prepare your audio files for pitch shifting.

To apply multiple pitch shifts at once, separate each factor by a comma, e.g. `-p 0.5,1,2`. Alternatively specify pitch shifting factors as semitones denoted by the + or - prefix, e.g. `-p -12,0,+12`. For precise pitch corrections append the number of cents after semitones, e.g. `-p -11-100,0,+11+100`.

To enable the formant preservation feature specify a suitable *quefrency* value in milliseconds. Depending on the source signal, begin with a small value like `-q 1`. Generally, the *quefrency* value has to be smaller than the fundamental period, as reciprocal of the fundamental frequency, of the source signal.

At the moment the formant preservation doesn't seem to work well along with the multi pitch shifting and smaller pitch shifting factors. Further investigation is therefore necessary...

## Further reading

### Instantaneous frequency estimation

* [Fundamentals of Music Processing](http://www.music-processing.de) by Meinard Müller (section 8.2.1 in the second edition or [online](https://www.audiolabs-erlangen.de/resources/MIR/FMP/C8/C8S2_InstantFreqEstimation.html))
* [Digital Audio Effects](http://www.dafx.de) by Udo Zölzer (sections 7.3.1 and 7.3.5 in the second edition)

### Cepstrum analysis and formant changing

* [Digital Audio Effects](http://www.dafx.de) by Udo Zölzer (sections 8.2.3 and 8.3.2 in the second edition)
* [Discrete-Time Signal Processing](https://www.pearson.com/us/higher-education/program/Oppenheim-Discrete-Time-Signal-Processing-3rd-Edition/PGM212808.html) by Oppenheim & Schafer (chapter 13 in the third edition)

## Credits

* [cxxopts](https://github.com/jarro2783/cxxopts) by Jarryd Beck
* [dr_libs](https://github.com/mackron/dr_libs) by David Reid

## License

*stftPitchShift* is licensed under the terms of the MIT license.
For details please refer to the accompanying [LICENSE](LICENSE) file distributed with *stftPitchShift*.
