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
    /// @brief Generates vertices for a sphere with texture coordinates and positions.
	//---------------------------------------------------------------------------------------------
    class SEGFAULT_EXPORT SphereGenerator final {
    public:
        /// @brief Constructs a new SphereGenerator.
        SphereGenerator() = default;

        /// @brief Destroys the SphereGenerator.
        ~SphereGenerator() = default;

        /// @brief Deleted copy constructor.
        SphereGenerator(const SphereGenerator&) = delete;

        /// @brief Deleted copy assignment operator.
        SphereGenerator& operator=(const SphereGenerator&) = delete;

        /// @brief Generates a sphere mesh with the specified parameters.
        /// @param radius The radius of the sphere. Defaults to 1.0f.
        /// @param sectors The number of sectors (longitude divisions). Defaults to 16.
        /// @param stacks The number of stacks (latitude divisions). Defaults to 16.
        /// @param color The color to apply to all vertices. Defaults to white (1.0f, 1.0f, 1.0f).
        /// @return A Mesh object containing the sphere's vertices and indices.
        Mesh generate(
            float radius = 1.0f,
            uint32_t sectors = 16,
            uint32_t stacks = 16,
            const glm::vec3& color = glm::vec3(1.0f)) const;

    private:
        /// @brief Converts spherical coordinates to Cartesian coordinates.
        /// @param radius The radius of the sphere.
        /// @param theta The azimuthal angle (longitude) in radians.
        /// @param phi The polar angle (latitude) in radians.
        /// @return The Cartesian position as a glm::vec3.
        glm::vec3 sphericalToCartesian(float radius, float theta, float phi) const;
    };

} // namespace segfault::renderer
