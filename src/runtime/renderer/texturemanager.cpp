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
#include "texturemanager.h"
#include "vulkanbuffer.h"
#include "rendercore.h"

#include "core/segfault.h"
#include "core/segfaultexception.h"

#include <stb_image.h>

#include <cassert>
#include <cstring>

namespace segfault::renderer {

    using namespace segfault::core;

    namespace {

        /// @brief Returns the number of bytes per texel for the formats the manager uploads.
        uint32_t bytesPerTexel(VkFormat format) {
            switch (format) {
                case VK_FORMAT_R8_UNORM:
                case VK_FORMAT_R8_SRGB:
                    return 1;
                case VK_FORMAT_R8G8_UNORM:
                case VK_FORMAT_R8G8_SRGB:
                    return 2;
                case VK_FORMAT_R8G8B8A8_UNORM:
                case VK_FORMAT_R8G8B8A8_SRGB:
                case VK_FORMAT_B8G8R8A8_UNORM:
                case VK_FORMAT_B8G8R8A8_SRGB:
                    return 4;
                default:
                    // The renderer only uploads 8-bit-per-channel formats; assume RGBA8.
                    return 4;
            }
        }

    } // namespace

    TextureManager::~TextureManager() {
        shutdown();
    }

    bool TextureManager::init(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool) {
        if (physicalDevice == VK_NULL_HANDLE || device == VK_NULL_HANDLE || graphicsQueue == VK_NULL_HANDLE ||
                commandPool == VK_NULL_HANDLE) {
            logMessage(LogType::Error, "TextureManager::init called with an invalid Vulkan context.");
            return false;
        }

        mPhysicalDevice = physicalDevice;
        mDevice = device;
        mGraphicsQueue = graphicsQueue;
        mCommandPool = commandPool;

        return true;
    }

    void TextureManager::shutdown() {
        if (mDevice == VK_NULL_HANDLE) {
            mTextures.clear();
            return;
        }

        for (auto& entry : mTextures) {
            if (entry.second != nullptr) {
                destroyTexture(*entry.second);
            }
        }
        mTextures.clear();

        mPhysicalDevice = VK_NULL_HANDLE;
        mDevice = VK_NULL_HANDLE;
        mGraphicsQueue = VK_NULL_HANDLE;
        mCommandPool = VK_NULL_HANDLE;
    }

    Texture* TextureManager::loadTexture(const std::string& name, const std::string& filename) {
        if (Texture* cached = getTexture(name)) {
            return cached;
        }

        int texWidth{ 0 }, texHeight{ 0 }, texChannels{ 0 };
        stbi_uc* pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        if (pixels == nullptr) {
            logMessage(LogType::Error, "TextureManager: failed to load texture image from file.");
            return nullptr;
        }

        Texture* texture = createTexture(name, pixels, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight),
            VK_FORMAT_R8G8B8A8_SRGB);

        stbi_image_free(pixels);

        return texture;
    }

    Texture* TextureManager::createTexture(const std::string& name, const void* pixels, uint32_t width, uint32_t height,
            VkFormat format) {
        if (Texture* cached = getTexture(name)) {
            return cached;
        }

        if (mDevice == VK_NULL_HANDLE) {
            logMessage(LogType::Error, "TextureManager: createTexture called before init.");
            return nullptr;
        }

        if (pixels == nullptr || width == 0 || height == 0) {
            logMessage(LogType::Error, "TextureManager: createTexture called with invalid pixel data.");
            return nullptr;
        }

        const VkDeviceSize imageSize = static_cast<VkDeviceSize>(width) * height * bytesPerTexel(format);

        // Upload the pixel data through a host-visible staging buffer.
        VulkanBuffer stagingBuffer(mPhysicalDevice, mDevice);
        if (!stagingBuffer.init(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
            logMessage(LogType::Error, "TextureManager: failed to create staging buffer for texture upload.");
            return nullptr;
        }
        stagingBuffer.bind();
        stagingBuffer.map();
        stagingBuffer.copyTo(const_cast<void*>(pixels), imageSize);
        stagingBuffer.unmap();

        auto texture = std::make_unique<Texture>();
        texture->name = name;
        texture->width = width;
        texture->height = height;
        texture->format = format;

        createImage(width, height, format, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            texture->image, texture->memory);

        transitionImageLayout(texture->image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        copyBufferToImage(stagingBuffer.getBuffer(), texture->image, width, height);
        transitionImageLayout(texture->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        stagingBuffer.cleanup();

        texture->imageView = createImageView(texture->image, format, VK_IMAGE_ASPECT_COLOR_BIT);
        texture->sampler = createSampler();

        Texture* result = texture.get();
        mTextures[name] = std::move(texture);

        return result;
    }

    Texture* TextureManager::getTexture(const std::string& name) const {
        auto it = mTextures.find(name);
        if (it == mTextures.end()) {
            return nullptr;
        }

        return it->second.get();
    }

    bool TextureManager::hasTexture(const std::string& name) const {
        return mTextures.find(name) != mTextures.end();
    }

    bool TextureManager::releaseTexture(const std::string& name) {
        auto it = mTextures.find(name);
        if (it == mTextures.end()) {
            return false;
        }

        if (it->second != nullptr) {
            destroyTexture(*it->second);
        }
        mTextures.erase(it);

        return true;
    }

    void TextureManager::destroyTexture(Texture& texture) {
        if (texture.sampler != VK_NULL_HANDLE) {
            vkDestroySampler(mDevice, texture.sampler, nullptr);
            texture.sampler = VK_NULL_HANDLE;
        }
        if (texture.imageView != VK_NULL_HANDLE) {
            vkDestroyImageView(mDevice, texture.imageView, nullptr);
            texture.imageView = VK_NULL_HANDLE;
        }
        if (texture.image != VK_NULL_HANDLE) {
            vkDestroyImage(mDevice, texture.image, nullptr);
            texture.image = VK_NULL_HANDLE;
        }
        if (texture.memory != VK_NULL_HANDLE) {
            vkFreeMemory(mDevice, texture.memory, nullptr);
            texture.memory = VK_NULL_HANDLE;
        }
    }

    uint32_t TextureManager::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const {
        VkPhysicalDeviceMemoryProperties memProperties{};
        vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw SegfaultException("TextureManager: failed to find suitable memory type!");
    }

    void TextureManager::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
            VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) const {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(mDevice, &imageInfo, nullptr, &image) != VK_SUCCESS) {
            throw SegfaultException("TextureManager: failed to create image!");
        }

        VkMemoryRequirements memRequirements{};
        vkGetImageMemoryRequirements(mDevice, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(mDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
            throw SegfaultException("TextureManager: failed to allocate image memory!");
        }

        vkBindImageMemory(mDevice, image, imageMemory, 0);
    }

    VkImageView TextureManager::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) const {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView{ VK_NULL_HANDLE };
        if (vkCreateImageView(mDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            throw SegfaultException("TextureManager: failed to create image view!");
        }

        return imageView;
    }

    VkSampler TextureManager::createSampler() const {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;

        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(mPhysicalDevice, &properties);
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;

        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        VkSampler sampler{ VK_NULL_HANDLE };
        if (vkCreateSampler(mDevice, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
            throw SegfaultException("TextureManager: failed to create texture sampler!");
        }

        return sampler;
    }

    void TextureManager::transitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout) const {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage{};
        VkPipelineStageFlags destinationStage{};

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else {
            throw SegfaultException("TextureManager: unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        endSingleTimeCommands(commandBuffer);
    }

    void TextureManager::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) const {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = { width, height, 1 };

        vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        endSingleTimeCommands(commandBuffer);
    }

    VkCommandBuffer TextureManager::beginSingleTimeCommands() const {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = mCommandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer{ VK_NULL_HANDLE };
        vkAllocateCommandBuffers(mDevice, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void TextureManager::endSingleTimeCommands(VkCommandBuffer commandBuffer) const {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(mGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(mGraphicsQueue);

        vkFreeCommandBuffers(mDevice, mCommandPool, 1, &commandBuffer);
    }

} // namespace segfault::renderer
