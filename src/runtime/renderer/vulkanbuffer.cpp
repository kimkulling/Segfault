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
#include "vulkanbuffer.h"
#include "core/segfault.h"

#include <memory>
#include <cassert>
#include <string.h>

namespace segfault::renderer {
    namespace {

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

        VkBufferUsageFlags getUsageFlags(BufferUsage usageFlags) {
            VkBufferUsageFlags flags{ 0 };

			if ((uint32_t) usageFlags & (uint32_t) BufferUsage::TransferSourceBit) {
				flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			}
			if ((uint32_t)usageFlags & (uint32_t)BufferUsage::TransferDestinationBit) {
				flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			}
			if ((uint32_t)usageFlags & (uint32_t)BufferUsage::UniformTexelBufferBit) {
				flags |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
			}
			if ((uint32_t)usageFlags & (uint32_t)BufferUsage::StorageTexelBufferBit) {
				flags |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
			}
			if ((uint32_t)usageFlags & (uint32_t)BufferUsage::UniformBuffer) {
				flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			}
			if ((uint32_t)usageFlags & (uint32_t)BufferUsage::IndexBuffer) {
				flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			}
            if ((uint32_t)usageFlags & (uint32_t)BufferUsage::VertexBuffer) {
                flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            }
            if ((uint32_t)usageFlags & (uint32_t)   BufferUsage::IndexBuffer) {
                flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            }
            if ((uint32_t)usageFlags & (uint32_t)BufferUsage::UniformBuffer) {
                flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            }

            return flags;
        }
    }

    VulkanBuffer::VulkanBuffer(VkPhysicalDevice physicalDevice, VkDevice device) :
            mPhysicalDevice(physicalDevice), mDevice(device) {
        // empty
    }

    bool VulkanBuffer::init(VkDeviceSize size, BufferUsage usageFlags, uint32_t memoryPropertyFlags) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = getUsageFlags(usageFlags);
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(mDevice, &bufferInfo, nullptr, &mBuffer) != VK_SUCCESS) {
            core::logMessage(core::LogType::Error, "failed to create buffer!");
            return false;
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(mDevice, mBuffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(mPhysicalDevice, memRequirements.memoryTypeBits, memoryPropertyFlags);

        if (vkAllocateMemory(mDevice, &allocInfo, nullptr, &mMemory) != VK_SUCCESS) {
            core::logMessage(core::LogType::Error, "failed to allocate buffer memory!");
            return false;
        }

        return true;
    }

	bool VulkanBuffer::init(VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags properties) {
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usageFlags;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		if (vkCreateBuffer(mDevice, &bufferInfo, nullptr, &mBuffer) != VK_SUCCESS) {
            core::logMessage(core::LogType::Error, "failed to create buffer!");
			return false;
		}
		
        VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(mDevice, mBuffer, &memRequirements);
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(mPhysicalDevice, memRequirements.memoryTypeBits, properties);
		if (vkAllocateMemory(mDevice, &allocInfo, nullptr, &mMemory) != VK_SUCCESS) {
            core::logMessage(core::LogType::Error, "failed to allocate buffer memory!");
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
        assert(mMapped != nullptr && "Buffer must be mapped before copying data to it.");
        assert(size <= VK_WHOLE_SIZE && "Size must be less than or equal to VK_WHOLE_SIZE.");
        assert(size <= 0 || data != nullptr && "Data pointer must not be null when size is greater than 0.");

        if (mMapped != nullptr) {
            memcpy(mMapped, data, static_cast<size_t>(size));
        }
    }

} // namespace segfault::renderer
