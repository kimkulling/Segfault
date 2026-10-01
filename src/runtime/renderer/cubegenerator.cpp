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

#include "renderer/cubegenerator.h"

namespace segfault::renderer {

    void CubeGenerator::generateFace(
            VertexArray& vertices,
            IndexArray& indices,
            const glm::vec3& position,
            const glm::vec3& right,
            const glm::vec3& up,
            const glm::vec3& color,
            uint16_t indexOffset,
            bool reverseWinding) const {
        // Calculate the four corners of the face
        const glm::vec3 corner0 = position - right - up;
        const glm::vec3 corner1 = position + right - up;
        const glm::vec3 corner2 = position + right + up;
        const glm::vec3 corner3 = position - right + up;

        // Texture coordinates for the face (standard mapping)
        // Note: V coordinate is flipped to match Vulkan's texture coordinate system
        // where (0,0) is at the top-left
        const auto texCoord0 = glm::vec2(0.0f, 1.0f); // Top-left
        const auto texCoord1 = glm::vec2(1.0f, 1.0f); // Top-right
        const auto texCoord2 = glm::vec2(1.0f, 0.0f); // Bottom-right
        const auto texCoord3 = glm::vec2(0.0f, 0.0f); // Bottom-left

        // Add vertices for this face
        vertices.push_back({corner0, color, texCoord0}); // 0
        vertices.push_back({corner1, color, texCoord1}); // 1
        vertices.push_back({corner2, color, texCoord2}); // 2
        vertices.push_back({corner3, color, texCoord3}); // 3

        // Add indices for two triangles
        if (reverseWinding) {
            // Reversed winding (clockwise) for back, left, bottom faces
            // Triangle 1: 0, 2, 1
            // Triangle 2: 0, 3, 2
            indices.push_back(indexOffset + 0);
            indices.push_back(indexOffset + 2);
            indices.push_back(indexOffset + 1);

            indices.push_back(indexOffset + 0);
            indices.push_back(indexOffset + 3);
            indices.push_back(indexOffset + 2);
        } else {
            // Counter-clockwise winding (default) for front, right, top faces
            // Triangle 1: 0, 1, 2
            // Triangle 2: 0, 2, 3
            indices.push_back(indexOffset + 0);
            indices.push_back(indexOffset + 1);
            indices.push_back(indexOffset + 2);

            indices.push_back(indexOffset + 0);
            indices.push_back(indexOffset + 2);
            indices.push_back(indexOffset + 3);
        }
    }

    Mesh CubeGenerator::generate(float size, const glm::vec3& color) const {
        Mesh mesh;
        mesh.vertices.reserve(24); // 6 faces * 4 vertices per face
        mesh.indices.reserve(36);  // 6 faces * 6 indices per face (2 triangles)

        const float halfSize = size * 0.5f;

        // Define the six faces of the cube
        // Each face is defined by its position (center of face), normal, right, and up vectors

        // Front face (positive Z)
        generateFace(
            mesh.vertices,
            mesh.indices,
            glm::vec3(0.0f, 0.0f, halfSize),
            glm::vec3(1.0f, 0.0f, 0.0f) * halfSize,  // right
            glm::vec3(0.0f, 1.0f, 0.0f) * halfSize,  // up
            color,
            0,
            false); // index offset, reverseWinding=false

        // Back face (negative Z)
        generateFace(
            mesh.vertices,
            mesh.indices,
            glm::vec3(0.0f, 0.0f, -halfSize),
            glm::vec3(-1.0f, 0.0f, 0.0f) * halfSize, // right (reversed for back face)
            glm::vec3(0.0f, 1.0f, 0.0f) * halfSize,  // up
            color,
            4,
            false); // index offset: right vector is already mirrored, so no winding reversal needed

        // Right face (positive X)
        generateFace(
            mesh.vertices,
            mesh.indices,
            glm::vec3(halfSize, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -1.0f) * halfSize, // right (negative Z)
            glm::vec3(0.0f, 1.0f, 0.0f) * halfSize,  // up
            color,
            8,
            false); // index offset, reverseWinding=false

        // Left face (negative X)
        generateFace(
            mesh.vertices,
            mesh.indices,
            glm::vec3(-halfSize, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f) * halfSize,  // right (positive Z)
            glm::vec3(0.0f, 1.0f, 0.0f) * halfSize,  // up
            color,
            12,
            false); // index offset: right vector is already mirrored, so no winding reversal needed

        // Top face (positive Y)
        generateFace(
            mesh.vertices,
            mesh.indices,
            glm::vec3(0.0f, halfSize, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f) * halfSize,  // right
            glm::vec3(0.0f, 0.0f, -1.0f) * halfSize, // up (negative Z for top face)
            color,
            16,
            false); // index offset, reverseWinding=false

        // Bottom face (negative Y)
        generateFace(
            mesh.vertices,
            mesh.indices,
            glm::vec3(0.0f, -halfSize, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f) * halfSize,  // right
            glm::vec3(0.0f, 0.0f, 1.0f) * halfSize,  // up (positive Z for bottom face)
            color,
            20,
            false); // index offset: right vector is already mirrored, so no winding reversal needed

        return mesh;
    }

} // namespace segfault::renderer
