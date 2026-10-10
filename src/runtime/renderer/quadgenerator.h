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
    ///	@ingroup    Runtime
    ///
    /// @brief Generates vertices for a flat quad in the XY plane (z = 0) for 2D rendering.
	//---------------------------------------------------------------------------------------------
    class SEGFAULT_EXPORT QuadGenerator final {
    public:
        /// @brief Constructs a new QuadGenerator.
        QuadGenerator() = default;

        /// @brief Destroys the QuadGenerator.
        ~QuadGenerator() = default;

        /// @brief Deleted copy constructor.
        QuadGenerator(const QuadGenerator&) = delete;

        /// @brief Deleted copy assignment operator.
        QuadGenerator& operator=(const QuadGenerator&) = delete;

        /// @brief Generates a quad mesh centered at the origin in the XY plane.
        /// @param width The width of the quad along the X axis. Defaults to 1.0f.
        /// @param height The height of the quad along the Y axis. Defaults to 1.0f.
        /// @param color The color to apply to all vertices. Defaults to white (1.0f, 1.0f, 1.0f).
        /// @return A Mesh object containing the quad's vertices and indices.
        Mesh generate(float width = 1.0f, float height = 1.0f,
            const glm::vec3& color = glm::vec3(1.0f)) const;
    };

} // namespace segfault::renderer
