#include "vulkandevice.h"

namespace segfault::renderer {

    bool QueueFamilyIndices::isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value() &&
               computeFamily.has_value() && transferFamily.has_value();
    }

    bool QueueFamilyIndices::isGraphicsComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }

    VulkanDevice::VulkanDevice(VkInstance instance) : mInstance(instance) {
        // empty
    }

    VulkanDevice::~VulkanDevice() {

    }

    bool VulkanDevice::init(VkSurfaceKHR surface, const DeviceRequirements& requirements) {
        mSurface = surface;
        if (!pickPhysicalDevice(surface, requirements)) {
            return false;
        }

        loadDeviceProperties();
        loadQueueFamilies();
        loadAvailableExtensions();
    }

    void VulkanDevice::shutdown() {
        if (mDevice != VK_NULL_HANDLE) {
            vkDestroyDevice(mDevice, nullptr);
            mDevice = VK_NULL_HANDLE;
        }
    }

} // namespace segfault::renderer
