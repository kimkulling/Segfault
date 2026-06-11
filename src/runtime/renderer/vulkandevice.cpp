#include "vulkandevice.h"

namespace segfault::renderer {

    bool QueueFamilyIndices::isGraphicsComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }

    VulkanDevice::VulkanDevice(VkPhysicalDevice physicalDevice) : mPhysicalDevice(physicalDevice) {
        // empty
    }

    VulkanDevice::~VulkanDevice() {

    }

    bool VulkanDevice::init(VkSurfaceKHR surface, const DeviceRequirements& requirements) {
        mSurface = surface;

        return true;
    }

    void VulkanDevice::shutdown() {
        if (mDevice != VK_NULL_HANDLE) {
            vkDestroyDevice(mDevice, nullptr);
            mDevice = VK_NULL_HANDLE;
        }
    }

} // namespace segfault::renderer
