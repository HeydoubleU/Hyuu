Reduces point cloud.

Divides points into grid cells, keeping only the point closest to the center.


### `cell_size`
Size of each grid cell. Smaller cell means more points will be kept.

### `cell_unit`
Unit used for cell size.

- **Absolute** <br> Cell size in world units.

- **Relative** <br> Cell size as fraction of the bounding box size.

- **Avg. Distance** <br> Cell size as average distance between neighboring points. This is intended as a more robust alternative to `Relative` for point clouds with non-uniform density.

### `Avg Distance Accuracy`
To compute the exact average distance is expensive, instead this value is estimated by sampling a subset of points. This parameter controls the percentage of points used. Higher values are more accurate but cost more to compute.
- This only applies when `cell_unit` is `Avg. Distance`.

### `disorder`
Jitters the points for a less uniform result. This is only for filtering, the resulting point_positions is not affected.