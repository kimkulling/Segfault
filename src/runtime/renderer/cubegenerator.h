/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2026 Segfault by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#pragma once

#include "renderer/rendercore.h"

#include <glm/glm.hpp>

namespace segfault::renderer {

	//---------------------------------------------------------------------------------------------
    /// @class CubeGenerator
    /// @brief Generates vertices for a cube with texture coordinates and positions.
	//---------------------------------------------------------------------------------------------
    class SEGFAULT_EXPORT CubeGenerator final {
    public:
        /// @brief Constructs a new CubeGenerator.
        CubeGenerator() = default;

        /// @brief Destroys the CubeGenerator.
        ~CubeGenerator() = default;

        /// @brief Deleted copy constructor.
        CubeGenerator(const CubeGenerator&) = delete;

        /// @brief Deleted copy assignment operator.
        CubeGenerator& operator=(const CubeGenerator&) = delete;

        /// @brief Generates a cube mesh with the specified size.
        /// @param size The size of the cube (edge length). Defaults to 1.0f.
        /// @param color The color to apply to all vertices. Defaults to white (1.0f, 1.0f, 1.0f).
        /// @return A Mesh object containing the cube's vertices and indices.
        Mesh generate(float size = 1.0f, const glm::vec3& color = glm::vec3(1.0f)) const;

    private:
        /// @brief Generates the vertices for a cube face.
        /// @param vertices The vertex array to append to.
        /// @param indices The index array to append to.
        /// @param position The center position of the face.
        /// @param right The right vector (tangent) of the face.
        /// @param up The up vector (bitangent) of the face.
        /// @param color The color to apply to the face vertices.
        /// @param indexOffset The starting index for this face's vertices.
        /// @param reverseWinding If true, reverses the triangle winding order (for back, left, bottom faces).
        void generateFace(
            VertexArray& vertices,
            IndexArray& indices,
            const glm::vec3& position,
            const glm::vec3& right,
            const glm::vec3& up,
            const glm::vec3& color,
            uint16_t indexOffset,
            bool reverseWinding = false) const;
    };

} // namespace segfault::renderer
