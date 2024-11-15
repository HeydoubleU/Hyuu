#include "../Hyuu.h"

namespace Hyuu {
	namespace Math {
		HYUU_DECL
			void sort_members(const Bifrost::Math::float2& input, Bifrost::Math::float2& sorted)
			AMINO_ANNOTATE("Amino::Node");

		HYUU_DECL
			void sort_members(const Bifrost::Math::float3& input, Bifrost::Math::float3& sorted)
			AMINO_ANNOTATE("Amino::Node");

		HYUU_DECL
			void sort_members(const Bifrost::Math::float4& input, Bifrost::Math::float4& sorted)
			AMINO_ANNOTATE("Amino::Node");

		HYUU_DECL
			void sort_members(const Bifrost::Math::long2& input, Bifrost::Math::long2& sorted)
			AMINO_ANNOTATE("Amino::Node");

		HYUU_DECL
			void sort_members(const Bifrost::Math::long3& input, Bifrost::Math::long3& sorted)
			AMINO_ANNOTATE("Amino::Node");

		HYUU_DECL
			void sort_members(const Bifrost::Math::long4& input, Bifrost::Math::long4& sorted)
			AMINO_ANNOTATE("Amino::Node");

	}

	namespace Random {
		HYUU_DECL
			void random_nondeterminstic(const Amino::float_t& type, Amino::float_t& random)
			AMINO_ANNOTATE("Amino::Node");

		HYUU_DECL
			void random_nondeterminstic(const Amino::long_t& type, Amino::long_t& random)
			AMINO_ANNOTATE("Amino::Node");

		HYUU_DECL
			void random_nondeterminstic(const Amino::uint_t& type, Amino::uint_t& random)
			AMINO_ANNOTATE("Amino::Node");

		HYUU_DECL
			void random_nondeterminstic(const Amino::int_t& type, Amino::int_t& random)
			AMINO_ANNOTATE("Amino::Node");
	}
}