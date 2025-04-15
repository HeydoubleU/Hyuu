Creates tags based on edge angle. Edge refers to the face edge corresponding to each face_vertex.

### `use_signed_angle`
For an edge to be considered "hard" the angle between it's faces must be greater than the `angle_threshold`. If `use_signed_angle` is `True`, different thresholds can be set for concave/convex edges.

### `tag_open_edges`
If `True`, open edges are tagged as hard.

### `point_hard`
A point is considered hard if 3 or more of its edges are hard. This is useful in the context of catmull-clark subdivision or remeshing, where such points are considered "corners" and should be preserved.