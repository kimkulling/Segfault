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

#include "vulkantypes.h"

namespace segfault::renderer {

	//---------------------------------------------------------------------------------------------
	/// @class VulkanBuffer
	/// @brief The VulkanBuffer class encapsulates a Vulkan buffer and its associated memory, providing
	/// a convenient interface for buffer management in Vulkan applications. It allows for the creation,
	/// mapping, binding, and copying of data to the buffer, as well as cleanup of
	/// resources when they are no longer needed.
	//---------------------------------------------------------------------------------------------
    class VulkanBuffer {
    public:
        VulkanBuffer(VkPhysicalDevice physicalDevice, VkDevice device);
        ~VulkanBuffer() = default;

        bool init(VkDeviceSize size, BufferUsage usageFlags, uint32_t memoryPropertyFlags);
        bool init(VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags properties);
        void cleanup();

        void map();
        void unmap();
        void bind(VkDeviceSize offset = 0);
        void copyTo(void *data, VkDeviceSize size);
        const VkBuffer &getBuffer() const { return mBuffer; }
        const VkDeviceMemory &getMemory() const { return mMemory; }
		size_t getSize() const { return mSize; }

    private:
        VkPhysicalDevice mPhysicalDevice{};
        VkDevice mDevice{};
        VkBuffer mBuffer{};
        VkDeviceMemory mMemory{};
        void *mMapped{nullptr};
        size_t mSize{0};
    };

} // namespace segfault::renderer
