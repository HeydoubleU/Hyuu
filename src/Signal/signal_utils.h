#include <vector>
#include <complex>
//#include <cmath>
//#include <algorithm>
#include "../Hyuu.h"


#define PI 3.14159265358979323846

// Limit windows to sizes within these exponents. This is to ensure accurate
// transforms on the low end and exploding user's pc on the high end.
#define WINDOW_SIZE_MIN 5
#define WINDOW_SIZE_MAX 24

namespace FFT {
    using Complex = std::complex<double>;
    using ComplexVector = std::vector<Complex>;

    constexpr int powerToWindowSize(int p) {
		p = std::max(WINDOW_SIZE_MIN, std::min(p, WINDOW_SIZE_MAX));
        return 1 << p; // efficient 2^p
    }

    inline float hanningFunction(int i, int window_size) {
        return 0.5f * (1.0f - std::cos(2.0f * PI * i / (window_size - 1)));
    }

	constexpr int roundToSquare(int n) {
		if (n < 1) return 1;
		int power = 1;
		while (power < n) {
			power <<= 1;
		}
		return power;
	}


    void fftRecursive(ComplexVector& x) {
        size_t n = x.size();
        if (n <= 1) return;

        // Divide
        ComplexVector even(n / 2), odd(n / 2);
        for (size_t i = 0; i < n / 2; ++i) {
            even[i] = x[i * 2];
            odd[i] = x[i * 2 + 1];
        }

        // Conquer
        fftRecursive(even);
        fftRecursive(odd);

        // Combine
        for (size_t i = 0; i < n / 2; ++i) {
            Complex t = std::polar(1.0, -2.0 * PI * i / n) * odd[i];
            x[i] = even[i] + t;
            x[i + n / 2] = even[i] - t;
        }
    }


    ComplexVector fft(const Array<float_t>& signal, long_t window_size, long_t sample_index, bool hanning)
    {
        //window_size = roundToSquare(window_size);  Assuming window_size is already a power of 2

        // Limit iterations to available samples
        int start = std::max(0LL, sample_index);
        int end = std::min(static_cast<long_t>(signal.size()), sample_index + window_size);
        int local_start = std::max(0LL, -sample_index);
        int local_end = (end - start) + local_start;

        // Constructor handles padding if iters < window_size
        FFT::ComplexVector fd(window_size);

        if (hanning) {
            for (int i = local_start; i < local_end; ++i) {
                fd[i] = FFT::Complex(signal[sample_index + i] * hanningFunction(i, window_size), 0.0);
            }
        }
        else {
            for (int i = local_start; i < local_end; ++i) {
                fd[i] = FFT::Complex(signal[sample_index + i], 0.0);
            }
        }

        FFT::fftRecursive(fd);
        return fd;
    }

	Array<float_t> ifft(FFT::ComplexVector& fd)
	{
		size_t window_size = fd.size();

        // Inverse FFT //
        for (auto& val : fd) {  // conjugate
            val = std::conj(val);
        }

        // Forward FFT
        FFT::fftRecursive(fd);

        // Conjugate output and scale
        Array<float_t> result(window_size);
        for (size_t i = 0; i < window_size; ++i) {
            result[i] = static_cast<float>(std::conj(fd[i]).real() / window_size);
        }

		return result;
	}


	Array<float_t> extractSpectrum(const FFT::ComplexVector& fd)
    {
        // Only half contains real (positive freq) magnitudes.
        Array<float_t> spectrum(fd.size() / 2);

        for (size_t i = 0; i < spectrum.size(); ++i) {
            spectrum[i] = static_cast<float>(std::abs(fd[i]));
        }

		return spectrum;
	}


    Array<float_t> extractSpectrumNormalized(const FFT::ComplexVector& fd, bool hanning)
    {
        // Only half contains real (positive freq) magnitudes.
        Array<float_t> spectrum(fd.size() / 2);

        // To normalize you'd multiply each mag by 1/fd.size(), but since I'm only using real numbers
        // the second half holds redundant information and is discarded, and therefore the actual
        // value I normalize with is 2/fd.size(). This is my reasoning at least so imma do it.
		float normalize_factor = 2.f / fd.size();

		if (hanning) {
			// Account for amplidude lost to hanning window, area under the curve is 0.5.
			normalize_factor *= 2.f;
		}

		for (size_t i = 0; i < spectrum.size(); ++i) {
			spectrum[i] = static_cast<float>(std::abs(fd[i]) * normalize_factor);
		}


        return spectrum;
    }


    void applyEq(ComplexVector& freq_domain, const std::vector<float>& factors)
    {
        size_t n = freq_domain.size() / 2;

        // scale frequencies
        for (size_t i = 0; i < n && i < factors.size(); ++i) {
            freq_domain[i] *= factors[i];
        }

        // maintain conjugate symmetry for real signals, whatever that means
        for (size_t i = 1; i < n && (n - i) < factors.size(); ++i) {
            size_t negFreqIndex = freq_domain.size() - i;
            freq_domain[negFreqIndex] *= factors[i];
        }
    }
}