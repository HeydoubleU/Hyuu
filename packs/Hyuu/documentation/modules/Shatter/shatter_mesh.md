Voronoi mesh shatter compound.


### `centers`
Points representing the center of each voronoi cell, from which each shard is created.

### `properties`
Properties to preserve from the original mesh. Currently only point properties are supported.

### `tag_components`
Generate point_cut and face_cut tags for where geometry is cut.

### `update_centers`
Shift centers to better align with their resulting shards, either based on the shards bounding box or sphere.

### `translate_to_origin`
Translate shaders to the origin. NOTE: In either case a `translate` property is create on each shard which stores its translate value.