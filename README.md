# smbPitchShift

This is a reimplementation of the Stephan M. Bernsee [smbPitchShift.cpp](https://blogs.zynaptiq.com/bernsee/download), a pitch shifting algorithm using the Short-Time Fourier Transform ([STFT](https://www.audiolabs-erlangen.de/resources/MIR/FMP/C2/C2_STFT-Basic.html)).

This repository features two analogical algorithm implementations, [main.cpp](main.cpp) (C++) and [main.py](main.py) (Python). Both of them contain a set of modules of the same name (but different file extension, of course).

In addition to the base algorithm implementation, it also features a spectral multi pitch shifting extension, which will be described in the later section.

## Modules

### Vocoder

The *Vocoder* module transforms the DFT spectral data according to the original algorithm, which is actually the *instantaneous frequency estimation* technique. See also [further reading](#further-reading) for more details.

The particular `encode` function replaces the input DFT values by the `magnitude + j * frequency` complex numbers, representing the phase error based frequency estimation in the imaginary part.

The `decode` function does an inverse transformation back to the original DFT complex numbers, by replacing eventually modified frequency value by the reconstructed phase value.

### Resampler

The *Resampler* module provides `linear` and `bilinear` interpolation routines, to actually perform pitch shifting, based on the *Vocoder* DFT transform.

### STFT

As the name of this module already implies, it performs the comprehensive *STFT* analysis and synthesis steps.

### IO

The *IO* module provides a simple possibility to read and write `.wav` audio files.

Currently only mono `.wav` files are supported. Please use e.g. [Audacity](http://www.audacityteam.org) or [SoX](http://sox.sourceforge.net) to prepare your audio files for pitch shifting.

## Pitch shifting

### Single pitch

Since the *Vocoder* module transforms the original DFT complex values `real + j * imag` into `magnitude + j * frequency` representation, the single pitch shifting is a quiet simple task. Both `magnitude` and `frequency` vectors are to be resampled according to the desired pitch shifting factor:

* The factor `1` means no change.
* The factor `<1` means downsampling.
* The factor `>1` means upsampling.

A fractional resampling factor like `0.5` requires interpolation. In the simplest case, linear interpolation will be sufficient. Otherwise, bilinear interpolation can also be applied, to smooth values between two consecutive STFT hops.

Due to frequency vector alteration, the resampled frequency values needs to be additionally multiplied by the resampling factor.

### Multi pitch

In terms of multi pitch shifting, multiple differently resampled `magnitude` and `frequency` vectors are to be combined together. For example, the magnitude vectors can be simply averaged. But what about the frequency vectors?

The basic concept of the algorithm extension is to only keep the frequency value of the strongest magnitude value. Since the strongest magnitude will mask the weakest one. Thus, the remaining masked frequency values are inaudible and can be omitted.

## Build

Use [CMake](http://cmake.org) to build the C++ program, e.g. `mkdir build && cmake .. && cmake --build .`.

The Python program, depending only on `click`, `matplotlib` and `numpy`, can be executed directly via `python3 main.py`.

## Usage

Both programs C++ and Python provides a similar set of command line arguments:

```
-h  --help     print this help

-i  --input    input .wav file name
-o  --output   output .wav file name

-p  --pitch    fractional pitch shifting factors separated by comma
               (default 1.0)

-w  --window   sfft window size
               (default 1024)

-v  --overlap  stft window overlap
               (default 32)

-d  --debug    plot spectrograms before and after processing
               (only available in the Python version)

    --smb      enable original algorithm
               (only available in the C++ version)
```

To apply multiple pitch shifts at once, separate each factor by a comma, e.g. `-p 0.5,1,2`.

## Further reading

* [Fundamentals of Music Processing](http://www.music-processing.de) by Meinard Müller (section 8.2.1 in the second edition or [online](https://www.audiolabs-erlangen.de/resources/MIR/FMP/C8/C8S2_InstantFreqEstimation.html))
* [Digital Audio Effects](http://www.dafx.de) by Udo Zölzer (sections 7.3.1 and 7.3.5 in the second edition)

## Credits

* [anyoption](https://github.com/hackorama/AnyOption) by Kishan Thomas
* [dr_libs](https://github.com/mackron/dr_libs) by David Reid
* [pocketfft](https://gitlab.mpcdf.mpg.de/mtr/pocketfft) by Martin Reinecke
* [smbPitchShift.cpp](http://blogs.zynaptiq.com/bernsee/download) by Stephan M. Bernsee
