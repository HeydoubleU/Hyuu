#include "../Hyuu.h"

using FaceEdge = Bifrost::Geometry::Mesh::FaceEdge;
using float3 = Bifrost::Math::float3;


namespace Hyuu {
    namespace Geometry {
        namespace Mesh {
            HYUU_DECL
                void chose_collapse_edge(
                    const Amino::long_t& point_id,
                    const Array<float3>& point_position,
					const Array<uint_t>& face_vertex,
					const Array<uint_t>& face_offset,
					const Array<FaceEdge>& fv_adj,
					const Array<FaceEdge>& point_adj,
					const Array<uint_t>& point_adj_offset,
					const Array<bool>& edge_creased,
					FaceEdge& shortest_edge,
                    float& edge_length,
					bool& collapsable
                ) AMINO_ANNOTATE("Amino::Node");

			HYUU_DECL
				void get_face_edge_unique_internal(
					const Array<uint_t>& face_offset,
					const Array<FaceEdge>& fv_adj,
					ArrayPtr<FaceEdge>& unique_edge,
					ArrayPtr<uint_t>& fv_unique_edge_id
				) AMINO_ANNOTATE("Amino::Node");

        }
    }
}