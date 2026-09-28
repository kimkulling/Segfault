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

#include "renderer/spheregenerator.h"

#include <cmath>

namespace segfault::renderer {

	static constexpr float PI = 3.14159265358979323846f;

    glm::vec3 SphereGenerator::sphericalToCartesian(float radius, float theta, float phi) const {
        // Convert spherical coordinates to Cartesian
        // theta: azimuthal angle (longitude) in [0, 2*PI]
        // phi: polar angle (latitude) in [0, PI]
        const float sinPhi = std::sin(phi);
        const float cosPhi = std::cos(phi);
        const float sinTheta = std::sin(theta);
        const float cosTheta = std::cos(theta);

        return glm::vec3(
            radius * cosTheta * sinPhi,   // x
            radius * cosPhi,               // y
            radius * sinTheta * sinPhi    // z
        );
    }

    Mesh SphereGenerator::generate(
        float radius,
        uint32_t sectors,
        uint32_t stacks,
        const glm::vec3& color) const {
        Mesh mesh;

        // Ensure at least 2 sectors and 2 stacks
        sectors = std::max(2u, sectors);
        stacks = std::max(2u, stacks);

        // Calculate the number of vertices and indices
        const uint32_t vertexCount = (stacks + 1) * (sectors + 1);
        const uint32_t indexCount = stacks * sectors * 6; // 2 triangles per sector-stack

        mesh.vertices.reserve(vertexCount);
        mesh.indices.reserve(indexCount);

        // Generate vertices
        for (uint32_t stack = 0; stack <= stacks; ++stack) {
            const float phi = static_cast<float>(PI) * stack / static_cast<float>(stacks);

            for (uint32_t sector = 0; sector <= sectors; ++sector) {
                const float theta = 2.0f * static_cast<float>(PI) * sector / static_cast<float>(sectors);

                // Calculate position using spherical coordinates
                glm::vec3 position = sphericalToCartesian(radius, theta, phi);

                // Calculate texture coordinates
                // s = sector / sectors (normalized longitude)
                // t = stack / stacks (normalized latitude, flipped for Vulkan)
                const float s = static_cast<float>(sector) / static_cast<float>(sectors);
                const float t = static_cast<float>(stack) / static_cast<float>(stacks);

                mesh.vertices.push_back({position, color, glm::vec2(s, 1.0f - t)});
            }
        }

        // Generate indices
        for (uint32_t stack = 0; stack < stacks; ++stack) {
            for (uint32_t sector = 0; sector < sectors; ++sector) {
                // Calculate vertex indices for this sector-stack
                const uint32_t current = stack * (sectors + 1) + sector;
                const uint32_t nextSector = stack * (sectors + 1) + (sector + 1);
                const uint32_t nextStack = (stack + 1) * (sectors + 1) + sector;
                const uint32_t nextStackNextSector = (stack + 1) * (sectors + 1) + (sector + 1);

                // First triangle (counter-clockwise)
                mesh.indices.push_back(static_cast<uint16_t>(current));
                mesh.indices.push_back(static_cast<uint16_t>(nextSector));
                mesh.indices.push_back(static_cast<uint16_t>(nextStackNextSector));

                // Second triangle (counter-clockwise)
                mesh.indices.push_back(static_cast<uint16_t>(current));
                mesh.indices.push_back(static_cast<uint16_t>(nextStackNextSector));
                mesh.indices.push_back(static_cast<uint16_t>(nextStack));
            }
        }

        return mesh;
    }

} // namespace segfault::renderer
