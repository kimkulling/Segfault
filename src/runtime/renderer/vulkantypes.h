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

#include "rendercore.h"

#include <cstdint>
#include <optional>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace segfault::renderer {

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily{};
        std::optional<uint32_t> presentFamily{};
        std::optional<uint32_t> computeFamily;
        std::optional<uint32_t> transferFamily;

        bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };


	/// @brief Represents a Vulkan command buffer and its associated command pool.
	struct CommandBuffer {
		VkCommandBuffer commandBuffer{};  ///< Vulkan command buffer handle.
		VkCommandPool commandPool{};      ///< Vulkan command pool handle.
	
		void setCommandBuffer(VkCommandBuffer cmdBuffer) {
			commandBuffer = cmdBuffer;
		}

		void setCommandPool(VkCommandPool cmdPool) {
			commandPool = cmdPool;
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities{};
		std::vector<VkSurfaceFormatKHR> formats{};
		std::vector<VkPresentModeKHR> presentModes{};
	};

} // namespace segfault::renderer
