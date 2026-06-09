#pragma once

#include "volk.h"


namespace segfault::renderer {

    enum class BufferUsage : uint32_t {
        VertexBuffer = 0x1,
        IndexBuffer = 0x2,
        UniformBuffer = 0x4
    };

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

    private:
        VkPhysicalDevice mPhysicalDevice{};
        VkDevice mDevice{};
        VkBuffer mBuffer{};
        VkDeviceMemory mMemory{};
        void *mMapped{nullptr};
    };

} // namespace segfault::renderer
