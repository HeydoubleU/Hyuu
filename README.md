Bifrost Version: 2.12.0.0

General purpose Bifrost library. Much of the library is lower level utilities for building compounds. To highlight some of the high level compounds:

### `Hyuu::Geometry::Mesh`

#### `remesh`
  > Similar to maya's remesh function. Produces evenly triangulated mesh given a target range of edge lengths. Supports feature preservation and open meshes.

#### `cut_mesh`
> Improved cut mesh compound, better point property transfer, now tags faces/points along cut.

#### `catmull_clark_subdivide_mesh`
> Improved catmull clark subd compound, significantly faster, now supports edge creasing.

#### `filter_mesh_property_data`
> smooth/erode/dilate mesh property

### `Hyuu::OpenCL`
GPU compute in the graph via OpenCL, nodes for building and executing OpenCL programs.

### `Hyuu::Image::Filter`
Compounds for imaging filtering on the cpu/gpu.

### `Hyuu::Shatter`
`shatter_mesh` and `post_process_shatter` compounds for shattering a mesh then adding detail to the result.

### `Hyuu::Containers`
unordered_map and unordered_set implemented in Bifrost.

### `Hyuu::Fur`
Preliminary library for fur/hair creation and styling.


## Install

Copy the `Hyuu` folder in `Compounds` to your prefered compounds directory. Add the path for `HyuuPackConfig.json` to `BIFROST_LIB_CONFIG_FILES`, e.g.:

```
BIFROST_LIB_CONFIG_FILES = C:/your/compounds/path/Hyuu/HyuuPackConfig.json
```

If the environment variable is already set you can append a path with using `;`

```
BIFROST_LIB_CONFIG_FILES = <existing value>;C:/your/compounds/path/Hyuu/HyuuPackConfig.json
```
