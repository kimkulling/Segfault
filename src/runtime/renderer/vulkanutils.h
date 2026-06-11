#pragma once

#include "volk.h"

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace segfault::renderer {

        struct Vertex {
        glm::vec3 pos{};
        glm::vec3 color{};
        glm::vec2 texCoord{};

        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
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
