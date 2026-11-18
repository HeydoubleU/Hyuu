cmake_minimum_required(VERSION 3.20)

# Ignore any machine-wide vcpkg toolchain.
unset( CMAKE_TOOLCHAIN_FILE CACHE )

if( NOT BIFROST_LOCATION )
    if( DEFINED ENV{BIFROST_LOCATION} )
        set( BIFROST_LOCATION $ENV{BIFROST_LOCATION})
    else()
        message( FATAL_ERROR "Bifrost: BIFROST_LOCATION cmake variable must be defined.")
    endif()
endif()

# Project Name
project(
    Hyuu
    VERSION 3.0
    LANGUAGES CXX
)

# Bifrost setup and utilities...
include(${BIFROST_LOCATION}/sdk/cmake/setup.cmake)

# Verbose CMake
set( CMAKE_VERBOSE_MAKEFILE TRUE)

# Find Bifrost
find_package(Bifrost REQUIRED SDK)

# Only needed for running inish_pack.py
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Build from sources
add_subdirectory(src)

# Run finish_pack.py
get_filename_component( BUILD_FOLDER_NAME "${CMAKE_BINARY_DIR}" NAME )
install( CODE "
    execute_process(
        COMMAND \"${Python3_EXECUTABLE}\" \"${CMAKE_SOURCE_DIR}/finish_pack.py\"
            --build-folder \"${BUILD_FOLDER_NAME}\"
            --pack-name \"${PROJECT_NAME}\"
            --pack-version \"${PROJECT_VERSION}\"
        WORKING_DIRECTORY \"${CMAKE_SOURCE_DIR}\"
        RESULT_VARIABLE _finish_pack_result
    )
    if( NOT _finish_pack_result EQUAL 0 )
        message( FATAL_ERROR \"finish_pack.py failed with exit code \${_finish_pack_result}\" )
    endif()
" )
