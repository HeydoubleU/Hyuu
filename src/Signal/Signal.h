#include "../Hyuu.h"


namespace Hyuu {
	namespace Signal {
		namespace Audio {
			HYUU_DECL
				void read_audio_wave(Amino::String filename, bool& success, ArrayPtr<float_t>& frames, long_t& sample_rate, long_t& channel_count, long_t& frame_count)
				AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/signal_icon.png}]");
		}  // namespace Audio

		HYUU_DECL void window_size_limits(int& min, int& max) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/signal_icon.png}]");
		HYUU_DECL float linear_to_db(float input) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/signal_icon.png}]");
		HYUU_DECL float linear_to_db_unclamped(float input) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/signal_icon.png}]");
		HYUU_DECL float db_to_linear(float input) AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/signal_icon.png}]");

		HYUU_DECL
			void compute_spectrum(
				const Array<float_t>& signal,
				long_t sample_rate,
				long_t sample_index,
				long_t window_size,
				bool hanning_window AMINO_ANNOTATE("Amino::Port value=true"),
				bool normalize AMINO_ANNOTATE("Amino::Port value=true"),
				ArrayPtr<float_t>& spectrum,
				float& freq_step)
			AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/signal_icon.png}]");

		HYUU_DECL
			void spectral_equalizer(
				const Array<float_t>& signal,
				const Array<float_t>& freq_factor,
				long_t sample_rate,
				long_t sample_index,
				bool hanning_window,
				ArrayPtr<float_t>& signal_window)
			AMINO_ANNOTATE("Amino::Node metadata=[{icon, ../icons/signal_icon.png}]");
	}
}