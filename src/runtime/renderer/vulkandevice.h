#pragma once

#include "volk.h"
#include "core/segfault.h"
#include <vector>
#include <optional>
#include <string>
#include <cstdint>

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
        VulkanDevice(VkInstance instance);
        ~VulkanDevice();

        /// @brief Initializes the device with the given surface and requirements.
        /// @param surface The presentation surface (can be VK_NULL_HANDLE for headless).
        /// @param requirements Device feature and extension requirements.
        /// @return True if device initialization succeeded.
        bool init(VkSurfaceKHR surface, const DeviceRequirements& requirements = {});

        /// @brief Shuts down the device and releases all resources.
        void shutdown();

        /// @brief Waits for all device operations to complete.
        void waitIdle() const;

        // Accessors
        VkInstance getInstance() const { return mInstance; }
        VkPhysicalDevice getPhysicalDevice() const { return mPhysicalDevice; }
        VkDevice getDevice() const { return mDevice; }
        VkQueue getGraphicsQueue() const { return mGraphicsQueue; }
        VkQueue getPresentQueue() const { return mPresentQueue; }
        const QueueFamilyIndices& getQueueFamilyIndices() const { return mQueueFamilyIndices; }
        uint32_t getGraphicsQueueFamilyIndex() const;
        uint32_t getPresentQueueFamilyIndex() const;
        const DeviceProperties& getProperties() const { return mProperties; }

        // Queue submission helpers
        bool submitGraphics(VkCommandBuffer cmdBuffer, VkFence fence = VK_NULL_HANDLE) const;
        bool submitCompute(VkCommandBuffer cmdBuffer, VkFence fence = VK_NULL_HANDLE) const;
        bool submitTransfer(VkCommandBuffer cmdBuffer, VkFence fence = VK_NULL_HANDLE) const;

        // Memory helpers
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

        // Device selection
        static std::vector<VkPhysicalDevice> enumeratePhysicalDevices(VkInstance instance);
        static bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const DeviceRequirements& requirements);
        static int rateDevice(VkPhysicalDevice device, const DeviceRequirements& requirements);

    private:
        VkInstance mInstance{};
        VkPhysicalDevice mPhysicalDevice{};
        VkDevice mDevice{};
        VkQueue mGraphicsQueue{};
        VkQueue mPresentQueue{};
        QueueFamilyIndices mQueueFamilyIndices{};
        DeviceProperties mProperties{};
        VkSurfaceKHR mSurface{};

        // Initialization helpers
        bool pickPhysicalDevice(VkSurfaceKHR surface, const DeviceRequirements& requirements);
        bool createLogicalDevice(const DeviceRequirements& requirements);
        void loadDeviceProperties();
        void loadQueueFamilies();
        void loadAvailableExtensions();
    };

} // namespace segfault::renderer
