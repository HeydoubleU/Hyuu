#include "Math.h"
#include <algorithm>
#include <tuple>
#include <random>


template<typename T>
void sort_two_members(const T& input, T& sorted) {
	if (input.x < input.y) {
		sorted.x = input.x;
		sorted.y = input.y;
	}
	else {
		sorted.x = input.y;
		sorted.y = input.x;
	}
}


template<typename T>
void sort_three_members(const T& input, T& sorted) {
	sorted = input;
	if (sorted.x > sorted.y) std::swap(sorted.x, sorted.y);
	if (sorted.y > sorted.z) std::swap(sorted.y, sorted.z);
	if (sorted.x > sorted.y) std::swap(sorted.x, sorted.y);
}


template<typename T>
void sort_four_members(const T& input, T& sorted) {
	sorted = input;
	if (sorted.x > sorted.y) std::swap(sorted.x, sorted.y);
	if (sorted.y > sorted.z) std::swap(sorted.y, sorted.z);
	if (sorted.z > sorted.w) std::swap(sorted.z, sorted.w);
	if (sorted.x > sorted.y) std::swap(sorted.x, sorted.y);
	if (sorted.y > sorted.z) std::swap(sorted.y, sorted.z);
	if (sorted.x > sorted.y) std::swap(sorted.x, sorted.y);
}


void Hyuu::Math::sort_members(const Bifrost::Math::float2& input, Bifrost::Math::float2& sorted) {
	sort_two_members(input, sorted);
}


void Hyuu::Math::sort_members(const Bifrost::Math::float3& input, Bifrost::Math::float3& sorted) {
	sort_three_members(input, sorted);
}


void Hyuu::Math::sort_members(const Bifrost::Math::float4& input, Bifrost::Math::float4& sorted) {
	sort_four_members(input, sorted);
}


void Hyuu::Math::sort_members(const Bifrost::Math::long2& input, Bifrost::Math::long2& sorted) {
	sort_two_members(input, sorted);
}


void Hyuu::Math::sort_members(const Bifrost::Math::long3& input, Bifrost::Math::long3& sorted) {
	sort_three_members(input, sorted);
}


void Hyuu::Math::sort_members(const Bifrost::Math::long4& input, Bifrost::Math::long4& sorted) {
	sort_four_members(input, sorted);
}

float Hyuu::Math::hermite_interp(float a, float b, float a_slope, float b_slope, float t) {
	float t2 = t * t;
	float t3 = t2 * t;

	return	(2 * t3 - 3 * t2 + 1) * a
			+ (t3 - 2 * t2 + t) * a_slope
			+ (-2 * t3 + 3 * t2) * b
			+ (t3 - t2) * b_slope;
}


float Hyuu::Math::catmull_rom_interp(float a, float b, float c, float d, float t) {
	float t2 = t * t;
	float t3 = t2 * t;

	return	0.5f * ((2 * b) +
			(-a + c) * t +
			(2 * a - 5 * b + 4 * c - d) * t2 +
			(-a + 3 * b - 3 * c + d) * t3);
}


void Hyuu::Math::rgb_hsv(const float3& rgb, float3& hsv) {
	float max = std::max({ rgb.x, rgb.y, rgb.z });
	float min = std::min({ rgb.x, rgb.y, rgb.z });
	hsv.z = max;

	if (max == min) {
		hsv.x = 0.0f;  // Undefined hue
		hsv.y = 0.0f;  // Saturation is zero
		return;
	}

	float delta = max - min;
	hsv.y = delta / max;

	if (max == rgb.x) {
		hsv.x = (rgb.y - rgb.z) / delta + (rgb.y < rgb.z ? 6.0f : 0.0f);
	}
	else if (max == rgb.y) {
		hsv.x = (rgb.z - rgb.x) / delta + 2.0f;
	}
	else {  // max == rgb.z
		hsv.x = (rgb.x - rgb.y) / delta + 4.0f;
	}

	hsv.x /= 6.0f;  // Normalize hue to [0, 1]
}


void Hyuu::Math::hsv_rgb(const float3& hsv, float3& rgb) {
	if (hsv.y == 0.0f) {
		rgb = { hsv.z, hsv.z, hsv.z };  // Grayscale
		return;
	}

	float h = hsv.x * 6.0f;  // Convert hue to [0, 6]
	int i = static_cast<int>(h);
	float f = h - i;  // Fractional part
	float p = hsv.z * (1.0f - hsv.y);
	float q = hsv.z * (1.0f - f * hsv.y);
	float t = hsv.z * (1.0f - (1.0f - f) * hsv.y);

	switch (i % 6) {
	case 0: rgb = { hsv.z, t, p }; break;
	case 1: rgb = { q, hsv.z, p }; break;
	case 2: rgb = { p, hsv.z, t }; break;
	case 3: rgb = { p, q, hsv.z }; break;
	case 4: rgb = { t, p, hsv.z }; break;
	case 5: rgb = { hsv.z, p, q }; break;
	}
}


void Hyuu::Math::circular_interpolate(float a, float b, float start, float end, float t, float& output) {
	float range = end - start;
	// Normalize a and b to [0, range)
	float na = fmodf(a - start + range, range);
	float nb = fmodf(b - start + range, range);

	float delta = fmodf((nb - na + range), range);
	if (delta > range / 2.0f) {
		delta -= range; // go the shorter way around
	}

	float result = fmodf((na + t * delta + range), range);
	output = result + start; // shift back to original range
}


// Random ======================================================================================================

void Hyuu::Random::random_nondeterminstic(const Amino::float_t& type, Amino::float_t& random) {
	random = std::random_device{}() / float(UINT32_MAX);
}

void Hyuu::Random::random_nondeterminstic(const Amino::long_t& type, Amino::long_t& random) {
	random = std::random_device{}();  // really only 32 bits of randomness, do I care?
	// random = std::random_device{}() | (std::random_device{}() << 32);  // 64 bits of randomness
}

void Hyuu::Random::random_nondeterminstic(const Amino::uint_t& type, Amino::uint_t& random) {
	random = std::random_device{}();  // really only 32 bits of randomness, do I care?
	// random = std::random_device{}() | (std::random_device{}() << 32);  // 64 bits of randomness
}

void Hyuu::Random::random_nondeterminstic(const Amino::int_t& type, Amino::int_t& random) {
	random = std::random_device{}();  // really only 32 bits of randomness, do I care?
	// random = std::random_device{}() | (std::random_device{}() << 32);  // 64 bits of randomness
}