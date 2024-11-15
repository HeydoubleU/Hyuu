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