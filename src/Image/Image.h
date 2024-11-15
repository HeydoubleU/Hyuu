#include "../Hyuu.h"

namespace Hyuu {
	namespace Image {
		namespace Filter::Kernel {
			struct AMINO_ANNOTATE("Amino::Struct") KernelMask {
				ArrayPtr<float> weights{ Amino::PtrDefaultFlag{} };
				uint2 extents;
			};
		}
		


		HYUU_DECL
			void pixel_kernel_indices(const uint_t& pixel, const Bifrost::Math::uint2& pixel_dimensions, const Bifrost::Math::uint2& extents, ArrayPtr<uint_t>& kernel_indices)
			AMINO_ANNOTATE("Amino::Node");
		
		HYUU_DECL
			void filter_pixel(
				const Array<float4>& pixel_color,
				const Array<uint_t>& kernel_indices,
				const Array<float>& kernel_weights,
				float4& filtered_color
			)
			AMINO_ANNOTATE("Amino::Node");
	}
}