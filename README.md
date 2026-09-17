My general purpose Bifrost library. Some highlights:
- Variety of lower level array, logic, string, math, and utility compounds.
- Variety of compounds for generating and modifying geometry and its properties.
- Adds C++ set and map containers to the graph.
- Signal module adds support for reading audio files and various signal processing compounds.
- OpenCL module adds operators for compiling and executing OpenCL code in the graph for GPU accelerated compounds.
- Terrain module adds terrain/heightmap creation and erosion compounds much of which is GPU accelerated with OpenCL.

This pack was built with Maya 2027 and Bifrost 3.3.0.0

## Install

The built pack is located in `/packs`, just copy the `Hyuu` folder to your `Compounds` folder. 

Optional: Open `HyuuPackConfig.json` and check the `bifrostVersion` field to verify it matches your current bifrost version.

## Building From Source

This project has only been tested on Windows. As far as I'm aware there's nothing to prevent this from building on other platforms.

Per the Bifrost SDK, building requires CMake and a Visual Studio toolset (on Windows). This project also assumes Python is installed, there's a post-build script `finish_pack.py` which assembles the pack automatically. With those requirements met, this project can be built with the following commands:
```

cmake --build build --config Release --targetcmake -S . -B build -G "Visual Studio 18 2026"      install
```
Or if the `BIFROST_LOCATION` environment is not set it can be provided explicitly:
```
cmake -S . -B build -G "Visual Studio 18 2026" -DBIFROST_LOCATION="<bifrost_location>"        
cmake --build build --config Release --target install
```

The final assembled pack will be at `/packs/Hyuu`.