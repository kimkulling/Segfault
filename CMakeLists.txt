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
    src/runtime
    src/contrib/cppcore/include
)

add_subdirectory(src/contrib/cppcore)
add_subdirectory(src/editor)
add_subdirectory(src/runtime)
add_subdirectory(src/tools)
add_subdirectory(src/examples)
