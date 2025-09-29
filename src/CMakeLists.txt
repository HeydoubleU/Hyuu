add_library(HyuuOps SHARED)

# OpenCL
find_package(OpenCL REQUIRED)
target_link_libraries(HyuuOps PRIVATE OpenCL::OpenCL)

# SndFile
find_package(SndFile CONFIG REQUIRED)
target_link_libraries(HyuuOps PRIVATE SndFile::sndfile)

# Build and install the library
target_compile_definitions( HyuuOps PRIVATE HYUU_BUILD_NODEDEF_DLL )
target_sources( HyuuOps PRIVATE Hyuu.cpp Containers/Containers.cpp Mesh/Mesh.cpp Math/Math.cpp Image/Image.cpp OpenCL/OpenCL.cpp Signal/Signal.cpp )

target_link_libraries( HyuuOps PUBLIC Amino::Cpp Amino::Core Bifrost::Object Bifrost::Geometry)
bifrost_set_install_rpath(HyuuOps)
install( TARGETS HyuuOps DESTINATION lib )

# Generate and install the json files
set( headers_to_parse 
    Hyuu.h
    Containers/Containers.h
    Mesh/Mesh.h
    Math/Math.h
    Image/Image.h
    OpenCL/OpenCL.h
    Signal/Signal.h
)

bifrost_header_parser( 
    HyuuJsonFiles  # Name of the target.
    "lib" # Install and build dirs - relative to install prefix
                           # and build directory
    INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
    HEADER_FILES ${headers_to_parse})

# Create config json file to load the bifrost pack
set( CONFIG_PACK_VERSION "${PROJECT_VERSION}")
set( CONFIG_PACK_NAME "${PROJECT_NAME}")
set( CONFIG_SHARED_LIBS "./lib")
set( CONFIG_JSON_FILES "./lib")
set( config_json ${CMAKE_BINARY_DIR}/${PROJECT_NAME}PackConfig.json )
configure_file(PackConfig.json.in ${config_json} )
install(FILES ${config_json} DESTINATION . )
