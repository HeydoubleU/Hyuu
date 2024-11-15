#include "Mesh.h"


inline bool isValidEdge(const FaceEdge& edge) {
    return edge.face != 4294967295;
}


inline bool isOpenFv(const uint_t& fv_id, const Array<FaceEdge>& fv_adj) {
    return !isValidEdge(fv_adj[fv_id]);
}


inline uint_t edgeFaceVertex(const FaceEdge& edge, const Array<uint_t>& face_offset) {
	return face_offset[edge.face] + edge.side;
}


inline FaceEdge getEdgeTwin(const FaceEdge& edge, const Array<FaceEdge>& fv_adj, const Array<uint_t>& face_offset) {
	return fv_adj[edgeFaceVertex(edge, face_offset)];
}


inline bool isOpenEdge(const FaceEdge& edge, const Array<FaceEdge>& fv_adj, const Array<uint_t>& face_offset) {
	return !isValidEdge(getEdgeTwin(edge, fv_adj, face_offset));
}


uint_t prevFaceVertex(const FaceEdge& edge, const Array<uint_t>& face_offset) {
	auto start = face_offset[edge.face];
	auto length = face_offset[edge.face + 1] - start;
	auto prev_side = length + edge.side - 1;
	prev_side = prev_side % length;
	return prev_side + start;
}

static FaceEdge prevEdge(const FaceEdge& edge, const Array<uint_t>& face_offset) {
    auto start = face_offset[edge.face];
    auto length = face_offset[edge.face + 1] - start;
    auto prev_side = length + edge.side - 1;
    prev_side = prev_side % length;

	auto prev_edge = FaceEdge();
	prev_edge.face = edge.face;
	prev_edge.side = prev_side;
	return prev_edge;
}


uint_t nextFaceVertex(const FaceEdge& edge, const Array<uint_t>& face_offset) {
	auto start = face_offset[edge.face];
	auto length = face_offset[edge.face + 1] - start;
	auto next_side = edge.side + 1;
	next_side = next_side % length;
	return next_side + start;
}


float edgeLength(const float3& p0, const float3& p1) {
	float x = p0.x - p1.x;
	float y = p0.y - p1.y;
	float z = p0.z - p1.z;
	return sqrt(x * x + y * y + z * z);
}


int valenceCount(const uint_t& point_id, const Array<uint_t>& point_adj_offset) {
	return point_adj_offset[point_id + 1] - point_adj_offset[point_id];
}


bool edgeNeighborsHaveEnoughValence(
	const FaceEdge& edge,
	const Array<uint_t>& face_vertex,
	const Array<uint_t>& face_offset,
	const Array<FaceEdge>& fv_adj,
	const Array<FaceEdge>& point_adj,  // not using but might need later
	const Array<uint_t>& point_adj_offset
) {
	auto prev_fv = prevFaceVertex(edge, face_offset);
	if (valenceCount(face_vertex[prev_fv], point_adj_offset) < 4) {
		return false;
	}

	auto twin = getEdgeTwin(edge, fv_adj, face_offset);
	if (!isValidEdge(twin)) {
		return true;
	}

	prev_fv = prevFaceVertex(twin, face_offset);
	if (valenceCount(face_vertex[prev_fv], point_adj_offset) < 4) {
		return false;
	}

	return true;
}


// Chose edge to collapse for point on an open edge, we only can collapse with other open edges
void chose_collapse_edge_open(
    uint_t start,
	uint_t end,
    const Amino::long_t& point_id,
    const Array<float3>& point_position,
    const Array<uint_t>& face_vertex,
    const Array<uint_t>& face_offset,
    const Array<FaceEdge>& fv_adj,
    const Array<FaceEdge>& point_adj,
	const Array<uint_t>& point_adj_offset,
    FaceEdge& shortest_edge,
    float& edge_length
) {
    const float3& p0 = point_position[point_id];
    float _length;

	for (auto i = start; i < end; i++) {
		auto edge = point_adj[i];
        auto fv_id = edgeFaceVertex(edge, face_offset);
		if (isOpenFv(fv_id, fv_adj)) {
			_length = edgeLength(p0, point_position[face_vertex[nextFaceVertex(edge, face_offset)]]);
		}
		else {
			edge = prevEdge(edge, face_offset);
			if (!isOpenFv(fv_id, fv_adj))
				continue;
			_length = edgeLength(p0, point_position[face_vertex[edgeFaceVertex(edge, face_offset)]]);
		}
		if (_length < edge_length) {
			if (!edgeNeighborsHaveEnoughValence(edge, face_vertex, face_offset, fv_adj, point_adj, point_adj_offset)) {
				continue;
			}
			edge_length = _length;
			shortest_edge = edge;
		}
	}

}

// Simply chose the shortest edge, this is for a point that is not creased or open.
void chose_collapse_edge_closed(
	uint_t start,
	uint_t end,
	const Amino::long_t& point_id,
	const Array<float3>& point_position,
	const Array<uint_t>& face_vertex,
	const Array<uint_t>& face_offset,
	const Array<FaceEdge>& fv_adj,
	const Array<FaceEdge>& point_adj,
	const Array<uint_t>& point_adj_offset,
	FaceEdge& shortest_edge,
	float& edge_length
) {
	const float3& p0 = point_position[point_id];
	float _length;

	for (auto i = start; i < end; i++) {
		auto edge = point_adj[i];
		auto fv_id = edgeFaceVertex(edge, face_offset);
		_length = edgeLength(p0, point_position[face_vertex[nextFaceVertex(edge, face_offset)]]);
		if (_length < edge_length) {
			if (!edgeNeighborsHaveEnoughValence(edge, face_vertex, face_offset, fv_adj, point_adj, point_adj_offset)) {
				continue;
			}
			edge_length = _length;
			shortest_edge = edge;
		}
	}

}


// Chose shortest creased edge.
void chose_collapse_edge_creased(
	uint_t start,
	uint_t end,
	const Amino::long_t& point_id,
	const Array<float3>& point_position,
	const Array<uint_t>& face_vertex,
	const Array<uint_t>& face_offset,
	const Array<FaceEdge>& fv_adj,
	const Array<FaceEdge>& point_adj,
	const Array<uint_t>& point_adj_offset,
	const Array<bool>& edge_creased,
	FaceEdge& shortest_edge,
	float& edge_length
) {
	const float3& p0 = point_position[point_id];
	float _length;

	for (auto i = start; i < end; i++) {
		auto edge = point_adj[i];
		auto fv_id = edgeFaceVertex(edge, face_offset);
		if (!edge_creased[fv_id])
			continue;
		_length = edgeLength(p0, point_position[face_vertex[nextFaceVertex(edge, face_offset)]]);
		if (_length < edge_length) {
			if (!edgeNeighborsHaveEnoughValence(edge, face_vertex, face_offset, fv_adj, point_adj, point_adj_offset)) {
				continue;
			}
			edge_length = _length;
			shortest_edge = edge;
		}
	}

}



void Hyuu::Geometry::Mesh::chose_collapse_edge(
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
) {
	collapsable = false;  // unless proven otherwise
	edge_length = -1;  // checked at the end to verify an edge was selected

	auto start = point_adj_offset[point_id];
    auto end = point_adj_offset[point_id + 1];
	auto length = end - start;
	uint_t crease_count = 0;
    bool is_open_point = false;

    for (auto i = start; i < end; i++) {
        const auto& edge = point_adj[i];
		auto fv_id = edgeFaceVertex(edge, face_offset);

        bool is_creased = edge_creased[fv_id];
		bool is_open = !isValidEdge(fv_adj[fv_id]);
        bool is_prev_open = !isValidEdge(fv_adj[prevFaceVertex(edge, face_offset)]);

		if (is_open || is_prev_open)
			is_open_point = true;
        if (is_creased && !is_open)
            crease_count++;  // creases don't count on open edge
    }

    // Under these conditions edge should not be collapsed
	if (crease_count > 2 || crease_count == 1 || (is_open_point && crease_count > 0)) {
		return;
	}

	edge_length = std::numeric_limits<float>::max();

	if (is_open_point) {
		chose_collapse_edge_open(start, end, point_id, point_position, face_vertex, face_offset, fv_adj, point_adj, point_adj_offset, shortest_edge, edge_length);
	}
	else if (crease_count == 2) { // creased point
		chose_collapse_edge_creased(start, end, point_id, point_position, face_vertex, face_offset, fv_adj, point_adj, point_adj_offset, edge_creased, shortest_edge, edge_length);
	}
	else {
		chose_collapse_edge_closed(start, end, point_id, point_position, face_vertex, face_offset, fv_adj, point_adj, point_adj_offset, shortest_edge, edge_length);
	}

	if (edge_length != -1)
		collapsable = true;
}



void Hyuu::Geometry::Mesh::get_face_edge_unique_internal(
	const Array<uint_t>& face_offset,
	const Array<FaceEdge>& fv_adj,
	ArrayPtr<FaceEdge>& unique_edge,
	ArrayPtr<uint_t>& fv_unique_edge_id
) {
	auto fv_count = fv_adj.size();
	auto face_count = face_offset.size() - 1;
	uint_t current_id = 0;
	auto _fv_unique_edge_id = Array<uint_t>(fv_count);
	auto _unique_edge = Array<FaceEdge>();
	FaceEdge edge;

	for (uint_t i = 0; i < face_count; i++) {
		const uint_t& start = face_offset[i];
		auto length = face_offset[i + 1] - start;
		for (uint_t side = 0; side < length; side++) {
			auto fv = start + side;
			const FaceEdge& adj_edge = fv_adj[fv];

			if (i > adj_edge.face) {  // loser beta edge
				continue;
			}

			edge.face = i;
			edge.side = side;
			_unique_edge.push_back(edge);
			_fv_unique_edge_id[fv] = current_id;
			if (isValidEdge(adj_edge)) {
				_fv_unique_edge_id[edgeFaceVertex(adj_edge, face_offset)] = current_id;
			}
			current_id++;
		}
	}

	unique_edge = Amino::newClassPtr<Amino::Array<FaceEdge>>(std::move(_unique_edge));
	fv_unique_edge_id = Amino::newClassPtr<Amino::Array<uint_t>>(std::move(_fv_unique_edge_id));
}
