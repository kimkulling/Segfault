#include "vulkandevice.h"

namespace segfault::renderer {

    bool QueueFamilyIndices::isGraphicsComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }

    VulkanDevice::VulkanDevice(VkPhysicalDevice physicalDevice) : mPhysicalDevice(physicalDevice) {
        // empty
    }

    VulkanDevice::~VulkanDevice() {
        shutdown();
    }

    bool VulkanDevice::init(VkSurfaceKHR surface, const DeviceRequirements& requirements) {
		if (mDevice != VK_NULL_HANDLE) {
			return true; // Already initialized
		}

        mSurface = surface;

        return true;
    }

    void VulkanDevice::shutdown() {
        if (mDevice == VK_NULL_HANDLE) {
            return;
        }

        vkDestroyDevice(mDevice, nullptr);
        mDevice = VK_NULL_HANDLE;
		mSurface = VK_NULL_HANDLE;
    }

    VulkanBuffer* VulkanDevice::createBuffer(size_t size, BufferUsage usageFlags, uint32_t memoryPropertyFlags) {
		VulkanBuffer* buffer = new VulkanBuffer(mPhysicalDevice, mDevice);
		if (!buffer->init(size, usageFlags, memoryPropertyFlags)) {
			delete buffer;
			return nullptr;
		}
		return buffer;
    }

    bool VulkanDevice::copyBuffer(VulkanBuffer* source, VulkanBuffer* destination, VkDeviceSize size, VkQueue queue, VkCommandPool pool) {
		if (!source || !destination) {
			return false;
		}

		VkCommandBuffer copyCmd = createCommandBuffer(pool);
		VkBufferCopy bufferCopy{};
		if (size == 0)
		{
			bufferCopy.size = source->getSize();
		}
		else
		{
			bufferCopy.size = size;
		}

		vkCmdCopyBuffer(copyCmd, source->getBuffer(), destination->getBuffer(), 1, &bufferCopy);

		flushCommandBuffer(copyCmd, queue, pool, true);
	}

	VkCommandBuffer VulkanDevice::createCommandBuffer(VkCommandPool commandPool, VkCommandBufferLevel level) {
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = level;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;

		vkAllocateCommandBuffers(mDevice, &allocInfo, &commandBuffer);
		return commandBuffer;
    }


	void VulkanDevice::flushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue, VkCommandPool pool, bool free) {
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;
		vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
		if (free) {
			vkFreeCommandBuffers(mDevice, pool, 1, &commandBuffer);
		}
	}

} // namespace segfault::renderer
