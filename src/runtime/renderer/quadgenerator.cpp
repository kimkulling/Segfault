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

#include "renderer/quadgenerator.h"

namespace segfault::renderer {

    Mesh QuadGenerator::generate(float width, float height, const glm::vec3& color) const {
        Mesh mesh;
        mesh.vertices.reserve(4); // single face * 4 vertices
        mesh.indices.reserve(6);  // single face * 6 indices (2 triangles)

        const float halfWidth = width * 0.5f;
        const float halfHeight = height * 0.5f;

        // The four corners of the quad in the XY plane (z = 0).
        // Winding and texture coordinates match the cube's front face so the
        // quad is visible with the same culling and orientation rules.
        const auto corner0 = glm::vec3(-halfWidth, -halfHeight, 0.0f); // Bottom-left
        const auto corner1 = glm::vec3(halfWidth, -halfHeight, 0.0f);  // Bottom-right
        const auto corner2 = glm::vec3(halfWidth, halfHeight, 0.0f);   // Top-right
        const auto corner3 = glm::vec3(-halfWidth, halfHeight, 0.0f);  // Top-left

        // Texture coordinates (V flipped to match Vulkan's top-left origin).
        const auto texCoord0 = glm::vec2(0.0f, 1.0f); // Bottom-left
        const auto texCoord1 = glm::vec2(1.0f, 1.0f); // Bottom-right
        const auto texCoord2 = glm::vec2(1.0f, 0.0f); // Top-right
        const auto texCoord3 = glm::vec2(0.0f, 0.0f); // Top-left

        mesh.vertices.emplace_back(Vertex{corner0, color, texCoord0}); // 0
        mesh.vertices.emplace_back(Vertex{corner1, color, texCoord1}); // 1
        mesh.vertices.emplace_back(Vertex{corner2, color, texCoord2}); // 2
        mesh.vertices.emplace_back(Vertex{corner3, color, texCoord3}); // 3

        // Two triangles with counter-clockwise winding.
        mesh.indices.emplace_back(0);
        mesh.indices.emplace_back(1);
        mesh.indices.emplace_back(2);

        mesh.indices.emplace_back(0);
        mesh.indices.emplace_back(2);
        mesh.indices.emplace_back(3);

        return mesh;
    }

} // namespace segfault::renderer
