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

#include <cstdint>
#include <optional>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace segfault::renderer {

    enum class BufferUsage : uint32_t {
		TransferSourceBit = 0x00000001,
		TransferDestinationBit = 0x00000002,
		UniformTexelBufferBit = 0x00000004,
        StorageTexelBufferBit = 0x00000008,
        UniformBuffer = 0x00000010,
        IndexBuffer = 0x00000040,
        VertexBuffer = 0x00000080,
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily{};
        std::optional<uint32_t> presentFamily{};
        std::optional<uint32_t> computeFamily;
        std::optional<uint32_t> transferFamily;

        bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };


    /// @brief Represents a vertex with position, color, and texture coordinates.
    struct Vertex {
        glm::vec3 pos{};        ///< Position of the vertex in 3D space.
        glm::vec3 color{};      ///< Color of the vertex (RGB).
        glm::vec2 texCoord{};   ///< Texture coordinates for the vertex.

        /// @brief Returns the attribute descriptions for the vertex.
        /// @return An array of VkVertexInputAttributeDescription for the vertex attributes.
        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();

        /// @brief Returns the binding description for the vertex.
        /// @return A VkVertexInputBindingDescription for the vertex binding.
        static VkVertexInputBindingDescription getBindingDescription();
    };

    using VertexArray = std::vector<Vertex>;    ///< Type alias for a vector of Vertex objects.
    using IndexArray = std::vector<uint16_t>;  ///< Type alias for a vector of index values.

	/// @brief Represents a mesh with vertices and indices.
    struct Mesh {
		VertexArray vertices;  ///< Array of vertices in the mesh.
		IndexArray indices;    ///< Array of indices for indexed drawing.

		void setVertices(const VertexArray& verts) {
			vertices = verts;
		}

		void setIndices(const IndexArray& inds) {
			indices = inds;
		}
	};

	/// @brief Represents a Vulkan command buffer and its associated command pool.
	struct CommandBuffer {
		VkCommandBuffer commandBuffer{};  ///< Vulkan command buffer handle.
		VkCommandPool commandPool{};      ///< Vulkan command pool handle.
	
		void setCommandBuffer(VkCommandBuffer cmdBuffer) {
			commandBuffer = cmdBuffer;
		}

		void setCommandPool(VkCommandPool cmdPool) {
			commandPool = cmdPool;
		}
	};

} // namespace segfault::renderer
