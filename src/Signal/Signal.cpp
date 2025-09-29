#define DR_WAV_IMPLEMENTATION
#include "dr_lib/dr_wav.h"
#include "Signal.h"
#include "signal_utils.h"


void Hyuu::Signal::Audio::read_audio_wave(Amino::String filename, bool& success, ArrayPtr<float_t>& frames, long_t& sample_rate, long_t& channel_count, long_t& frame_count)
{
	drwav wav;
	if (!drwav_init_file(&wav, filename.c_str(), nullptr)) {
		sample_rate = 0;
		channel_count = 0;
		frame_count = 0;
		frames = Amino::newClassPtr<Array<float_t>>();
		success = false;
		return;
	}

	sample_rate = wav.sampleRate;
	channel_count = wav.channels;
	frame_count = wav.totalPCMFrameCount;

	Array<float_t> _frames(frame_count * channel_count);
	drwav_read_pcm_frames_f32(&wav, frame_count, _frames.data());
	drwav_uninit(&wav);
	frames = Amino::newClassPtr<Array<float_t>>(std::move(_frames));

	success = true;
}


void Hyuu::Signal::window_size_limits(int& min, int& max)
{
	min = WINDOW_SIZE_MIN;
	max = WINDOW_SIZE_MAX;
}


float Hyuu::Signal::linear_to_db(float input)
{
	// Avoid log(0) or negative values
	return 20.0f * std::log10(std::max(input, std::numeric_limits<float>::min()));
}


float Hyuu::Signal::linear_to_db_unclamped(float input)
{
	// Avoid log(0) or negative values
	return 20.0f * std::log10(input);
}


float Hyuu::Signal::db_to_linear(float input)
{
	return std::pow(10.0f, input / 20.0f);
}


void Hyuu::Signal::compute_spectrum(
	const Array<float_t>& signal,
	long_t sample_rate,
	long_t sample_index,
	long_t window_exp,
	bool hanning_window,
	bool normalize,
	ArrayPtr<float_t>& spectrum,
	float& freq_step)
{
	long_t window_size = FFT::powerToWindowSize(window_exp);
	FFT::ComplexVector fd = FFT::fft(signal, window_size, sample_index, hanning_window);
	spectrum = Amino::newClassPtr<Array<float_t>>(std::move(
		normalize ? FFT::extractSpectrumNormalized(fd, hanning_window) : FFT::extractSpectrum(fd)
	));
	freq_step = static_cast<float>(sample_rate) / window_size;  // Frequency step in Hz

}


void Hyuu::Signal::spectral_equalizer(
	const Array<float_t>& signal,
	const Array<float_t>& freq_factor,
	long_t sample_rate,
	long_t sample_index,
	bool hanning_window,
	ArrayPtr<float_t>& signal_window)
{
	long_t window_size = FFT::roundToSquare(freq_factor.size() * 2);
	FFT::ComplexVector fd = FFT::fft(signal, window_size, sample_index, hanning_window);

	size_t n = window_size/2;

	// scale frequencies
	for (size_t i = 0; i < n && i < freq_factor.size(); ++i) {
		fd[i] *= freq_factor[i];
	}

	// maintain conjugate symmetry for real signals, whatever that means
	for (size_t i = 1; i < n && (n - i) < freq_factor.size(); ++i) {
		size_t sym_index = fd.size() - i;
		fd[sym_index] *= freq_factor[i];
	}

	signal_window = Amino::newClassPtr<Array<float_t>>(std::move(FFT::ifft(fd)));
}