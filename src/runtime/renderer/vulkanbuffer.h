#pragma once

#include "volk.h"

#include "vulkantypes.h"

namespace segfault::renderer {

    class VulkanBuffer {
    public:
        VulkanBuffer(VkPhysicalDevice physicalDevice, VkDevice device);
        ~VulkanBuffer() = default;

        VkDevice device{};
        bool init(size_t size, BufferUsage usageFlags, uint32_t memoryPropertyFlags);
        void cleanup();

        void map();
        void unmap();
        void bind(VkDeviceSize offset = 0);
        void copyTo(void *data, VkDeviceSize size);
        VkBuffer getBuffer() const { return mBuffer; }
        VkDeviceMemory getMemory() const { return mMemory; }
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
