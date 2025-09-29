#ifndef HYUU_H
#define HYUU_H

// Common includes
#include "HyuuExport.h"
#include <Amino/Cpp/ClassDeclare.h>
#include <Amino/Cpp/Annotate.h>
#include <Amino/Core/Any.h>
#include <Amino/Core/Array.h>
#include <Bifrost/Math/Types.h>
#include <Bifrost/Geometry/GeometryTypes.h>
#include <Bifrost/Object/Object.h>


using uint_t = Amino::uint_t;
using int_t = Amino::int_t;
using ulong_t = Amino::ulong_t;
using long_t = Amino::long_t;
template <typename T>
using Array = Amino::Array<T>;
template <typename T>
using ArrayPtr = Amino::Ptr<Amino::Array<T>>;
using float2 = Bifrost::Math::float2;
using float3 = Bifrost::Math::float3;
using float4 = Bifrost::Math::float4;
using int2 = Bifrost::Math::int2;
using int3 = Bifrost::Math::int3;
using int4 = Bifrost::Math::int4;
using uint2 = Bifrost::Math::uint2;
using uint3 = Bifrost::Math::uint3;
using uint4 = Bifrost::Math::uint4;
using uchar_t = Amino::uchar_t;


#endif // HYUU_H

namespace Hyuu {
	namespace File {
		HYUU_DECL void read_text_file(const Amino::String& filename, Amino::String& text) AMINO_ANNOTATE("Amino::Node");
		HYUU_DECL void write_text_file(const Amino::String& text, const Amino::String& filename, bool& success) AMINO_ANNOTATE("Amino::Node");

		HYUU_DECL void write_json(Bifrost::Object& object AMINO_ANNOTATE("Amino::InOut outName=object_out"),
			const Amino::String& filename, bool& success) AMINO_ANNOTATE("Amino::Node");
	}
}