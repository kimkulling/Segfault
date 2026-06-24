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

#include "volk.h"

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace segfault::renderer {

	/// @brief Represents a vertex with position, color, and texture coordinates.
    struct Vertex {
		glm::vec3 pos{};        ///< Position of the vertex in 3D space.
		glm::vec3 color{};      ///< Color of the vertex (RGB).
		glm::vec2 texCoord{};   ///< Texture coordinates for the vertex.

		/// @brief Returns the attribute descriptions for the vertex.
		/// @return An array of VkVertexInputAttributeDescription for the vertex attributes.
        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();

		/// @brief Returns the binding description for the vertex.
		/// @return A VkVertexInputBindingDescription for the vertex binding.
        static VkVertexInputBindingDescription getBindingDescription();
    };

    /// @brief Utility functions for Vulkan operations.
    class VulkanUtils {
    public:
        /// @brief Finds a supported format from a list of candidates.
        /// @param physicalDevice The physical device to query.
        /// @param candidates A list of candidate formats.
        /// @param tiling The desired image tiling.
        /// @param features The required format features.
        /// @return The first supported format from the candidates.
        static VkFormat findSupportedFormat(VkPhysicalDevice &physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        /// @brief Finds a supported depth format.
        /// @param physicalDevice The physical device to query.
        /// @return A supported depth format.
        static VkFormat findDepthFormat(VkPhysicalDevice &physicalDevice);
    };

} // namespace segfault::renderer
