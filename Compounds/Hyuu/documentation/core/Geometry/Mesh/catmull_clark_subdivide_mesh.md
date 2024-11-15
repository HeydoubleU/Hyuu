Subdivide and smooth mesh using the Catmull-Clark algorithm.

### `properties`
Only face properties are supported currently.

### `edge_crease`
per-face-edge crease value, for preserving sharp edges akin to maya's crease tool. If two face edge twins have different crease values, the max of the pair is used.