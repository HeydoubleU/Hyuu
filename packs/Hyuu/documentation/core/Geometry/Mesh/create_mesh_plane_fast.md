Optimized mesh plane creation with image related tasks/high division count in mind.

### `points_resolution`
If `False`, the `resolution` parameter behaves like the native "create_mesh_plane" compound, where the number represents the number of faces along each axis. if `True` the number represents the number of points along each axis. In this case, setting the resolution to that of some image, allows for setting the point_color using that image's pixel_color directly.