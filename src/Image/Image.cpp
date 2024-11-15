#include "Image.h"


void pixelWeightedSum(float4& to, const float4 color, const float weight) {
	to.x += color.x * weight;
	to.y += color.y * weight;
	to.z += color.z * weight;
	to.w += color.w * weight;
}


void Hyuu::Image::pixel_kernel_indices(const uint_t& pixel, const Bifrost::Math::uint2& pixel_dimensions, const Bifrost::Math::uint2& extents, ArrayPtr<uint_t>& kernel_indices)
{
	auto dim_x = extents.x * 2 + 1;
	auto dim_y = extents.y * 2 + 1;
    Array<uint_t> _kernel = Array<uint_t>();
	_kernel.reserve(dim_x * dim_y);

	const auto width = pixel_dimensions.x;
    const int x = pixel % width;
    const int y = pixel / pixel_dimensions.y;
	const int x_max = width - 1;
	const int y_max = pixel_dimensions.y - 1;

    // Iterate through kernel positions
    for (int ky = -static_cast<int>(extents.y); ky <= static_cast<int>(extents.y); ++ky) {
        for (int kx = -static_cast<int>(extents.x); kx <= static_cast<int>(extents.x); ++kx) {
            // Calculate target position with clamping
            int target_x = std::clamp(x + kx, 0, x_max);
            int target_y = std::clamp(y + ky, 0, y_max);
            uint_t index = target_y * width + target_x;
			_kernel.push_back(index);
        }
    }

    kernel_indices = Amino::newClassPtr<Array<uint_t>>(std::move(_kernel));
}


void Hyuu::Image::filter_pixel(
    const Array<float4>& pixel_color,
    const Array<uint_t>& kernel_indices,
	const Array<float>& kernel_weights,
	float4& filtered_color
    )
{
	filtered_color = float4();
	const auto kernel_size = kernel_indices.size();
	for (uint_t i = 0; i < kernel_size; ++i) {
		const auto& index = kernel_indices[i];
		pixelWeightedSum(filtered_color, pixel_color[index], kernel_weights[i]);
	}
}