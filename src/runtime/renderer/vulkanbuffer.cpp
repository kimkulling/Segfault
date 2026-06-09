#include "vulkanbuffer.h"
#include <memory>

namespace segfault::renderer {

    uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties{};
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        return 0;
    }

    VulkanBuffer::VulkanBuffer(VkPhysicalDevice physicalDevice, VkDevice device) :
            mPhysicalDevice(physicalDevice), mDevice(device) {
        // empty
    }

    VkBufferUsageFlags getUsageFlags(BufferUsage usageFlags) {
        VkBufferUsageFlags flags = 0;
        if (usageFlags == BufferUsage::VertexBuffer) flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        if (usageFlags == BufferUsage::IndexBuffer) flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        if (usageFlags == BufferUsage::UniformBuffer) flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        return flags;
    }

    bool VulkanBuffer::init(size_t size, BufferUsage usageFlags, uint32_t memoryPropertyFlags) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = getUsageFlags(usageFlags);
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(mDevice, &bufferInfo, nullptr, &mBuffer) != VK_SUCCESS) {
            return false;
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(mDevice, mBuffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(mPhysicalDevice, memRequirements.memoryTypeBits, memoryPropertyFlags);

        if (vkAllocateMemory(mDevice, &allocInfo, nullptr, &mMemory) != VK_SUCCESS) {
            return false;
        }
        
        return true;
    }

    void VulkanBuffer::cleanup() {
        unmap();

        if (mBuffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(mDevice, mBuffer, nullptr);
            mBuffer = VK_NULL_HANDLE;
        }

        if (mMemory != VK_NULL_HANDLE) {
            vkFreeMemory(mDevice, mMemory, nullptr);
            mMemory = VK_NULL_HANDLE;
        }
    }

    void VulkanBuffer::map() {
        vkMapMemory(mDevice, mMemory, 0, VK_WHOLE_SIZE, 0, &mMapped);
    }

    void VulkanBuffer::unmap() {
        if (mMapped != nullptr) {
            vkUnmapMemory(mDevice, mMemory);
            mMapped = nullptr;
        }
    }

    void VulkanBuffer::bind(VkDeviceSize offset) {
        vkBindBufferMemory(mDevice, mBuffer, mMemory, offset);
    }

    void VulkanBuffer::copyTo(void *data, VkDeviceSize size) {
        if (mMapped != nullptr) {
            memcpy(mMapped, data, static_cast<size_t>(size));
        }
    }

} // namespace segfault::renderer
