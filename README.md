# smbPitchShift

This is a reimplementation of the Stephan M. Bernsee [smbPitchShift.cpp](https://blogs.zynaptiq.com/bernsee/download), a pitch shifting algorithm using the Short-Time Fourier Transform ([STFT](https://www.audiolabs-erlangen.de/resources/MIR/FMP/C2/C2_STFT-Basic.html)).

This repository features two analogical algorithm implementations, [C++](main.cpp) and [Python](main.py). Both of them contains following modules of the same name (but different file extension, of course).

### Vocoder

The *Vocoder* module transforms the DFT spectral data according to the original algorithm, which is actually the [instantaneous frequency estimation](https://www.audiolabs-erlangen.de/resources/MIR/FMP/C8/C8S2_InstantFreqEstimation.html) technique.

The particular `encode` function replaces the input DFT values by the `magnitude + j * frequency` complex numbers, representing the phase error based frequency estimation in the imaginary part.

The `decode` function does an inverse transformation back to the original DFT complex numbers, by replacing eventually modified frequency value by the reconstructed phase value.

### Resampler

The *Resampler* module provides `linear` and `bilinear` interpolation routines, to actually perform pitch shifting, based on the *Vocoder* DFT transform.

### STFT

As the name of this module already implies, it performs the comprehensive *STFT* analysis and synthesis steps.

### IO

The *IO* module provides a simple possibility to read and write `.wav` audio files.

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

## Further reading

* [Fundamentals of Music Processing](http://www.music-processing.de) by Meinard Müller (Section 8.2.1 or [online](https://www.audiolabs-erlangen.de/resources/MIR/FMP/C8/C8S2_InstantFreqEstimation.html))
* [Digital Audio Effects](http://www.dafx.de) by Udo Zölzer (Sections 7.3.1 and 7.3.5)

## Credits

* [anyoption](https://github.com/hackorama/AnyOption) by Kishan Thomas
* [dr_libs](https://github.com/mackron/dr_libs) by David Reid
* [pocketfft](https://gitlab.mpcdf.mpg.de/mtr/pocketfft) by Martin Reinecke
* [smbPitchShift.cpp](http://blogs.zynaptiq.com/bernsee/download) by Stephan M. Bernsee
