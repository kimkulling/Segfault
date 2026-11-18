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

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

namespace segfault::renderer {

    /// @brief Describes a single GPU texture resource managed by the TextureManager.
    ///
    /// A Texture bundles the Vulkan image together with its backing memory, a default
    /// image view and a sampler so it can be bound to a descriptor set directly.
    struct Texture {
        std::string name;                   ///< Logical name / cache key of the texture.
        VkImage image{ VK_NULL_HANDLE };            ///< Vulkan image handle.
        VkDeviceMemory memory{ VK_NULL_HANDLE };    ///< Device memory backing the image.
        VkImageView imageView{ VK_NULL_HANDLE };    ///< Default image view for shader access.
        VkSampler sampler{ VK_NULL_HANDLE };        ///< Sampler used when reading the texture.
        uint32_t width{ 0 };                ///< Width of the texture in texels.
        uint32_t height{ 0 };               ///< Height of the texture in texels.
        VkFormat format{ VK_FORMAT_UNDEFINED };     ///< Pixel format of the image.

        /// @brief Returns true if the texture holds a valid, usable Vulkan image.
        bool isValid() const {
            return image != VK_NULL_HANDLE && imageView != VK_NULL_HANDLE;
        }
    };

    //---------------------------------------------------------------------------------------------
    /// @class TextureManager
    /// @brief Creates, caches and owns the lifetime of Vulkan textures for the renderer.
    ///
    /// The TextureManager encapsulates the boilerplate required to turn CPU pixel data into a
    /// sampleable Vulkan image: it uploads the data through a staging buffer, performs the
    /// required layout transitions and creates a matching image view and sampler. Textures are
    /// cached by name so that requesting the same resource twice returns the already uploaded
    /// instance instead of loading it again. All created resources are released on shutdown.
    //---------------------------------------------------------------------------------------------
    class TextureManager {
    public:
        /// @brief Constructs an uninitialized texture manager. Call init() before use.
        TextureManager() = default;

        /// @brief Destroys the manager and releases every texture it still owns.
        ~TextureManager();

        // Disallow copying, the manager owns GPU resources.
        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;

        /// @brief Initializes the manager with the Vulkan context it needs for uploads.
        /// @param physicalDevice The physical device used to query memory properties.
        /// @param device         The logical device that owns all created resources.
        /// @param graphicsQueue  Queue used to submit the upload / transition commands.
        /// @param commandPool    Command pool used to allocate the one-time upload commands.
        /// @return True if the manager was initialized successfully.
        bool init(VkPhysicalDevice physicalDevice, VkDevice device, VkQueue graphicsQueue, VkCommandPool commandPool);

        /// @brief Releases every managed texture and resets the manager to its initial state.
        void shutdown();

        /// @brief Loads a texture from an image file, uploading it to the GPU.
        ///
        /// The file is decoded to RGBA8 and uploaded. If a texture with the same name has
        /// already been loaded the cached instance is returned instead.
        /// @param name The cache key / logical name for the texture.
        /// @param filename The path to the image file on disk.
        /// @return A pointer to the owned texture, or nullptr on failure.
        Texture* loadTexture(const std::string& name, const std::string& filename);

        /// @brief Creates a texture from raw, tightly packed pixel data in memory.
        ///
        /// If a texture with the same name already exists the cached instance is returned.
        /// @param name   The cache key / logical name for the texture.
        /// @param pixels Pointer to the source pixel data (width * height * bytes-per-texel).
        /// @param width  Width of the image in texels.
        /// @param height Height of the image in texels.
        /// @param format Vulkan format describing the pixel layout (defaults to RGBA8 sRGB).
        /// @return A pointer to the owned texture, or nullptr on failure.
        Texture* createTexture(const std::string& name, const void* pixels, uint32_t width, uint32_t height,
            VkFormat format = VK_FORMAT_R8G8B8A8_SRGB);

        /// @brief Returns a previously created texture by name, or nullptr if unknown.
        Texture* getTexture(const std::string& name) const;

        /// @brief Returns true if a texture with the given name is currently cached.
        bool hasTexture(const std::string& name) const;

        /// @brief Destroys a single cached texture and removes it from the cache.
        /// @return True if a texture with that name existed and was released.
        bool releaseTexture(const std::string& name);

        /// @brief Returns the number of textures currently managed.
        size_t getTextureCount() const;

    private:
        /// @brief Destroys the Vulkan resources held by a texture.
        void destroyTexture(Texture& texture);

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
            VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) const;
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) const;
        VkSampler createSampler() const;
        void transitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout) const;
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) const;

        VkCommandBuffer beginSingleTimeCommands() const;
        void endSingleTimeCommands(VkCommandBuffer commandBuffer) const;

        VkPhysicalDevice mPhysicalDevice{ VK_NULL_HANDLE };
        VkDevice mDevice{ VK_NULL_HANDLE };
        VkQueue mGraphicsQueue{ VK_NULL_HANDLE };
        VkCommandPool mCommandPool{ VK_NULL_HANDLE };
        std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures{};
    };

    inline size_t TextureManager::getTextureCount() const {
        return mTextures.size();
    }

} // namespace segfault::renderer
