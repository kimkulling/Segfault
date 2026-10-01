#-----------------------------------------------------------------------------------------------
# The MIT License (MIT)
#
# Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
#-----------------------------------------------------------------------------------------------

# Add custom targets for build scripts

# Find Python interpreter
find_package(Python3 COMPONENTS Interpreter REQUIRED)

# Get the build configuration (Debug/Release)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_CONFIG "Debug")
else()
    set(BUILD_CONFIG "Release")
endif()

# Platform-specific output directory
if(WIN32)
    set(OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../bin/${BUILD_CONFIG}")
elseif(UNIX AND NOT APPLE)
    set(OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../bin")
else()
    set(OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../bin")
endif()

# Create output directories for shaders and textures
file(MAKE_DIRECTORY "${OUTPUT_DIR}/shaders")
file(MAKE_DIRECTORY "${OUTPUT_DIR}/textures")

# Custom target for compiling shaders
add_custom_target(compile_shaders
    COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/compile_shader.py"
        --shader "${CMAKE_CURRENT_SOURCE_DIR}/../assets/shaders/"
        --config "${BUILD_CONFIG}"
        --verbose
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Compiling shaders..."
)

# Custom target for compiling assets (copying textures)
add_custom_target(compile_assets
    COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/compile_assets.py"
        --textures "${CMAKE_CURRENT_SOURCE_DIR}/../assets/textures/"
        --config "${BUILD_CONFIG}"
        --verbose
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Compiling assets..."
)

# Custom target that runs both
add_custom_target(compile_all_assets
    DEPENDS compile_shaders compile_assets
    COMMENT "Compiling all assets..."
)
