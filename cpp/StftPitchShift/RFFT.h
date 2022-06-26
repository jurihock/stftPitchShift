#pragma once

#include <StftPitchShift/FFT.h>

#include <cassert>
#include <map>
#include <stdexcept>
#include <string>

namespace stftpitchshift
{
  template<class T>
  struct CRFFT
  {
    CRFFT(const size_t fullsize)
    {
      const bool is_power_of_two = fullsize && !(fullsize & (fullsize - 1));

      if (!is_power_of_two)
      {
        throw std::runtime_error(
          "The specified frame size " + std::to_string(fullsize) + " is not a power of 2! " +
          "Please specify values like 1024, 2048, 4096 etc.");
      }

      size.full = fullsize;
      size.half = fullsize / 2;

      buffer.resize(size.half);

      coeffs.w.resize(size.half);
      coeffs.a.resize(size.half);
      coeffs.b.resize(size.half);

      coeffs.conj.w.resize(size.half);
      coeffs.conj.a.resize(size.half);
      coeffs.conj.b.resize(size.half);

      const T pi = T(-2) * std::acos(T(-1));

      for (size_t i = 0; i < size.half; ++i)
      {
        coeffs.w[i] = std::polar(T(1), (pi * i) / T(size.half));
        coeffs.a[i] = imul({ T(0.5), T(0.5) }, -std::polar(T(1), (pi * i) / T(size.full)));
        coeffs.b[i] = imul({ T(0.5), T(0.5) }, +std::polar(T(1), (pi * i) / T(size.full)));

        coeffs.conj.w[i] = std::conj(coeffs.w[i]);
        coeffs.conj.a[i] = std::conj(coeffs.a[i]);
        coeffs.conj.b[i] = std::conj(coeffs.b[i]);
      }

      coeffs.p.resize(size.half);

      const size_t bits = static_cast<size_t>(std::log2(size.half));

      for (size_t i = 0; i < size.half; ++i)
      {
        coeffs.p[i] = rebit(i, bits);
      }
    }

    static std::complex<T> imul(const std::complex<T>& x, const std::complex<T>& y)
    {
      return // x.r + j * x.i * (y.r + j * y.i)
      {
        x.real() - x.imag() * y.imag(),
        x.imag() * y.real()
      };
    }

    static size_t rebit(size_t x, size_t bits)
    {
      size_t y = 0; // reversed bit order

      for (size_t i = 0; i < bits; ++i)
      {
        y <<= 1;
        y |= (x & 1);
        x >>= 1;
      }

      return y;
    }

    struct
    {
      size_t full = 0;
      size_t half = 0;
    }
    size;

    std::vector<std::complex<T>> buffer;

    struct
    {
      std::vector<size_t> p;

      std::vector<std::complex<T>> w, a, b;

      struct
      {
        std::vector<std::complex<T>> w, a, b;
      }
      conj;
    }
    coeffs;
  };
}

namespace stftpitchshift
{
  template<class T>
  class TRFFT
  {

  public:

    void fft(const std::vector<T>& frame, std::vector<std::complex<T>>& dft)
    {
      assert(dft.size() == frame.size() / 2 + 1);

      CRFFT<T>& cache = *(getcache(frame.size()).get());

      const size_t size = cache.size.half;

      auto input = reinterpret_cast<const std::complex<T>*>(frame.data());
      auto output = dft.data();
      auto buffer = cache.buffer.data();

      auto p = cache.coeffs.p.data();
      auto w = cache.coeffs.w.data();
      auto a = cache.coeffs.a.data();
      auto b = cache.coeffs.b.data();

      transform(input, buffer, p, w, size);
      pack(buffer, output, a, b, size);

      const T scale = T(1) / T(size);

      for (size_t i = 0; i < size; ++i)
      {
        output[i] *= scale;
      }
    }

    void ifft(const std::vector<std::complex<T>>& dft, std::vector<T>& frame)
    {
      assert(dft.size() == frame.size() / 2 + 1);

      CRFFT<T>& cache = *(getcache(frame.size()).get());

      const size_t size = cache.size.half;

      auto input = dft.data();
      auto output = reinterpret_cast<std::complex<T>* const>(frame.data());
      auto buffer = cache.buffer.data();

      auto p = cache.coeffs.p.data();
      auto w = cache.coeffs.conj.w.data();
      auto a = cache.coeffs.conj.a.data();
      auto b = cache.coeffs.conj.b.data();

      pack(input, buffer, a, b, size);
      transform(buffer, output, p, w, size);
    }

  private:

    std::map<size_t, std::shared_ptr<CRFFT<T>>> caches;

    std::shared_ptr<CRFFT<T>> getcache(const size_t size)
    {
      auto it = caches.find(size);

      if (it != caches.end())
      {
        return it->second;
      }

      auto cache = std::make_shared<CRFFT<T>>(size);

      caches.emplace(size, cache);

      return cache;
    }

    static void pack(const std::complex<T>* input, std::complex<T>* const output, const std::complex<T>* a, const std::complex<T>* b, const size_t size)
    {
      output[0] = input[0] * a[0] + std::conj(input[0]) * b[0];

      for (size_t i = 1, j = size - 1; i < size; ++i, --j)
      {
        output[i] = input[i] * a[i] + std::conj(input[j]) * b[i];
      }
    }

    static void transform(const std::complex<T>* input, std::complex<T>* const output, const size_t* p, const std::complex<T>* w, const size_t size)
    {
      const size_t bits = static_cast<size_t>(std::log2(size));

      for (size_t i = 0; i < size; ++i)
      {
        output[p[i]] = input[i];
      }

      for (size_t bit = 1; bit <= bits; ++bit)
      {
        const size_t n1 = size_t(1) << bit;
        const size_t n2 = n1 >> 1;

        const size_t inc = size / n1;

        for (size_t i = 0; i < size; i += n1)
        {
          for (size_t j = i, k = 0; j < i + n2; ++j, k += inc)
          {
            const auto left = output[j];
            const auto right = output[j + n2] * w[k];

            output[j] = left + right;
            output[j + n2] = left - right;
          }
        }
      }
    }
  };
}

namespace stftpitchshift
{
  class RFFT : public FFT
  {

  public:

    void fft(const std::vector<float>& frame, std::vector<std::complex<float>>& dft) override
    {
      precision.f.fft(frame, dft);
    }

    void fft(const std::vector<double>& frame, std::vector<std::complex<double>>& dft) override
    {
      precision.d.fft(frame, dft);
    }

    void ifft(const std::vector<std::complex<float>>& dft, std::vector<float>& frame) override
    {
      precision.f.ifft(dft, frame);
    }

    void ifft(const std::vector<std::complex<double>>& dft, std::vector<double>& frame) override
    {
      precision.d.ifft(dft, frame);
    }

  private:

    struct
    {
      TRFFT<float> f;
      TRFFT<double> d;
    }
    precision;

  };
}
