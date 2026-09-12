message(STATUS "The CMake version used is ${CMAKE_VERSION}")
cmake_minimum_required(VERSION 3.5...${CMAKE_VERSION})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

PROJECT(segfault
  VERSION 0.1.0
  DESCRIPTION ""
  HOMEPAGE_URL "")

# Cache these to allow the user to override them manually.
set( LIB_INSTALL_DIR "lib"      CACHE PATH "Path the built library files are installed to." )
set( BIN_INSTALL_DIR "bin"      CACHE PATH "Path the tool executables are installed to." )

# Set the default build output folders
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )

set(SEGFAULT_VERSION_MAJOR 0 )
set(SEGFAULT_VERSION_MINOR 1 )
set(SEGFAULT_VERSION_PATCH 0 )
set(SEGFAULT_VERSION ${SEGFAULT_VERSION_MAJOR}.${SEGFAULT_VERSION_MINOR}.${SEGFAULT_VERSION_PATCH} )
set(PROJECT_VERSION "${SEGFAULT_VERSION}" )
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2          CONFIG REQUIRED)
find_package(glm                  REQUIRED)
find_package(volk          CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

include_directories( BEFORE
    /usr/include/stb
    src/runtime
    src/contrib/cppcore/include
)

add_subdirectory(src/contrib/cppcore)
add_subdirectory(src/editor)
add_subdirectory(src/runtime)
add_subdirectory(src/tools)
add_subdirectory(src/examples)

# Add custom target to copy assets to the build output directory
# For multi-config generators (VS, Xcode), CMAKE_RUNTIME_OUTPUT_DIRECTORY is the base
# and executables go to subdirectories (Debug, Release, etc.)
# For single-config generators, CMAKE_RUNTIME_OUTPUT_DIRECTORY already includes the config path

if(CMAKE_CONFIGURATION_TYPES)
    # Multi-config generator (Visual Studio, Xcode)
    # Create a target for each configuration
    foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
        add_custom_target(copy_assets_${CONFIG}
            COMMAND ${CMAKE_COMMAND} -E echo "Copying assets for ${CONFIG} configuration..."
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIG}/assets"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets/shaders" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIG}/assets/shaders"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets/textures" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIG}/assets/textures"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets/manifests" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIG}/assets/manifests"
            COMMENT "Copying asset files for ${CONFIG} configuration"
        )
    endforeach()
    
    # Also create a copy_assets target that depends on all config-specific targets
    # This is for backward compatibility with examples/CMakeLists.txt
    add_custom_target(copy_assets ALL)
    foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
        add_dependencies(copy_assets copy_assets_${CONFIG})
    endforeach()
else()
    # Single-config generator (Make, Ninja)
    add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Copying assets to build directory..."
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets/shaders" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets/textures" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets/manifests" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/manifests"
        COMMENT "Copying asset files to build directory"
    )
endif()
