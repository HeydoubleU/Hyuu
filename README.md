My general purpose Bifrost library. Some highlights:
- Variety of lower level array, logic, string, math, and utility compounds.
- Variety of compounds for generating and modifying geometry and its properties.
- Adds C++ set and map containers to the graph.
- Signal module adds support for reading audio files and various signal processing compounds.
- OpenCL module adds operators for compiling and executing OpenCL code in the graph for GPU accelerated compounds.
- Terrain module adds terrain/heightmap creation and erosion compounds much of which is GPU accelerated with OpenCL.


## Install

Copy the `Hyuu` folder in `Compounds` to your prefered compounds directory. Add the path for `HyuuPackConfig.json` to `BIFROST_LIB_CONFIG_FILES`, e.g.:

```
BIFROST_LIB_CONFIG_FILES = C:/your/compounds/path/Hyuu/HyuuPackConfig.json
```

If the environment variable is already set you can append a path with using `;`

```
BIFROST_LIB_CONFIG_FILES = <existing value>;C:/your/compounds/path/Hyuu/HyuuPackConfig.json
```
