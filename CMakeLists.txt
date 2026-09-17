cmake_minimum_required(VERSION 3.20)

# This project has no vcpkg dependency, so ignore any machine-wide vcpkg
# toolchain CMake would otherwise auto-apply from the CMAKE_TOOLCHAIN_FILE
# environment variable. Must happen before project()/enable_language().
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

# Needed to run finish_pack.py after install (portably - not all platforms have 'python' on PATH)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Verbose CMake
set( CMAKE_VERBOSE_MAKEFILE TRUE)

# Find Bifrost
find_package(Bifrost REQUIRED SDK)

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
