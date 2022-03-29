# smbPitchShift

This is an interpretation of the Stephan M. Bernsee [smbPitchShift.cpp](https://blogs.zynaptiq.com/bernsee/download):

* `Vocoder.cpp` Spectral data processing according to the original algorithm
* `STFT.cpp` Short-Time Fourier Transform implementation
* `IO.cpp` Audio file import and export

## Usage

```
-h,?  --help     print this help

-i    --input    input .wav file name
-o    --output   output .wav file name

-p    --pitch    pitch shifting factor
                 (default 1.0)

-w    --window   sfft window size
                 (default 1024)

-v    --overlap  stft window overlap
                 (default 32)

      --smb      enable original algorithm
```

## Credits

* [anyoption](https://github.com/hackorama/AnyOption) by Kishan Thomas
* [dr_libs](https://github.com/mackron/dr_libs) by David Reid
* [pocketfft](https://gitlab.mpcdf.mpg.de/mtr/pocketfft) by Martin Reinecke
* [smbPitchShift.cpp](http://blogs.zynaptiq.com/bernsee/download) by Stephan M. Bernsee
