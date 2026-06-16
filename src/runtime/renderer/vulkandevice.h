#pragma once

#include "volk.h"
#include "core/segfault.h"
#include <vector>
#include <optional>
#include <string>
#include <cstdint>
#include "vulkantypes.h"
#include "vulkanbuffer.h"

namespace segfault::renderer {
    
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        std::optional<uint32_t> computeFamily;
        std::optional<uint32_t> transferFamily;

        bool isComplete() const;
        bool isGraphicsComplete() const;
    };

    struct DeviceRequirements {
        std::vector<const char*> requiredExtensions;
        std::vector<const char*> optionalExtensions;
        VkPhysicalDeviceFeatures requiredFeatures{};
        VkPhysicalDeviceFeatures optionalFeatures{};
        bool requirePresentQueue{ false };
        bool requireComputeQueue{ false };
        bool requireTransferQueue{ false };
    };

    struct DeviceProperties {
        VkPhysicalDeviceProperties properties{};
        VkPhysicalDeviceFeatures features{};
        VkPhysicalDeviceMemoryProperties memoryProperties{};
        std::vector<VkExtensionProperties> availableExtensions;
        std::vector<VkQueueFamilyProperties> queueFamilies;
    };

    //---------------------------------------------------------------------------------------------
    /// @class VulkanDevice
    /// @brief Encapsulates a Vulkan physical and logical device with queue management.
    ///
    /// This class manages the selection, creation, and lifecycle of Vulkan devices.
    /// It provides access to device properties, features, queues, and memory management.
    //---------------------------------------------------------------------------------------------
    class SEGFAULT_EXPORT VulkanDevice final {
    public:
        // Disallow copying
        VulkanDevice(const VulkanDevice&) = delete;
        VulkanDevice& operator=(const VulkanDevice&) = delete;

        /// @brief Constructs a VulkanDevice.
        /// @param instance The Vulkan instance.
        VulkanDevice(VkPhysicalDevice physicalDevice);

		/// @brief The class destructor. Ensures that all Vulkan resources are properly released.
        ~VulkanDevice();

        /// @brief Initializes the device with the given surface and requirements.
        /// @param surface The presentation surface (can be VK_NULL_HANDLE for headless).
        /// @param requirements Device feature and extension requirements.
        /// @return True if device initialization succeeded.
        bool init(VkSurfaceKHR surface, const DeviceRequirements& requirements = {});

        /// @brief Shuts down the device and releases all resources.
        void shutdown();

        // Accessors
        VkPhysicalDevice getPhysicalDevice() const { return mPhysicalDevice; }
        VkDevice getDevice() const { return mDevice; }
        VkQueue getGraphicsQueue() const { return mGraphicsQueue; }
        VkQueue getPresentQueue() const { return mPresentQueue; }
        const QueueFamilyIndices& getQueueFamilyIndices() const { return mQueueFamilyIndices; }
        uint32_t getGraphicsQueueFamilyIndex() const;
        uint32_t getPresentQueueFamilyIndex() const;
        const DeviceProperties& getProperties() const { return mProperties; }
		VulkanBuffer* createBuffer(size_t size, BufferUsage usageFlags, uint32_t memoryPropertyFlags);
        bool copyBuffer(VulkanBuffer *source, VulkanBuffer *destination, VkDeviceSize size, VkQueue queue, VkCommandPool pool);
        VkCommandBuffer createCommandBuffer(VkCommandPool commandPool, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
        void flushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue, VkCommandPool pool, bool free);

    private:
        VkPhysicalDevice mPhysicalDevice{};
        VkDevice mDevice{};
        VkQueue mGraphicsQueue{};
        VkQueue mPresentQueue{};
        QueueFamilyIndices mQueueFamilyIndices{};
        DeviceProperties mProperties{};
        VkSurfaceKHR mSurface{};
    };

} // namespace segfault::renderer
