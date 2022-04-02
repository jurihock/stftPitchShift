# smbPitchShift

This is a reimplementation of the Stephan M. Bernsee [smbPitchShift.cpp](https://blogs.zynaptiq.com/bernsee/download), a pitch shifting algorithm using the Short-Time Fourier Transform ([STFT](https://en.wikipedia.org/wiki/Short-time_Fourier_transform)).

This repository features two analogical algorithm implementations, [C++](main.cpp) and [Python](main.py). Both of them contains following modules of the same name (but different file extension, of course).

### Vocoder

The Vocoder module transforms the DFT spectral data according to the original algorithm. The particular `encode` function replaces the input DFT values by the `magnitude + j * frequency` complex numbers. The `decode` function does an inverse transformation back to the original DFT complex numbers.

### Resampler

The Resampler module provides `linear` and `bilinear` functions, to actually perform the pitch shifting, based on the Vocoder DFT transform.

### STFT

As the name of this module already implies, it performs the comprehensive STFT analysis and synthesis steps.

### IO

The IO module provides a simple possibility to read and write `.wav` audio files.

## Usage

```
-h  --help     print this help

-i  --input    input .wav file name
-o  --output   output .wav file name

-p  --pitch    pitch shifting factor
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

## Credits

* [anyoption](https://github.com/hackorama/AnyOption) by Kishan Thomas
* [dr_libs](https://github.com/mackron/dr_libs) by David Reid
* [pocketfft](https://gitlab.mpcdf.mpg.de/mtr/pocketfft) by Martin Reinecke
* [smbPitchShift.cpp](http://blogs.zynaptiq.com/bernsee/download) by Stephan M. Bernsee
