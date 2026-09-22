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

#include "core/segfault.h"
#include "volk.h"

#include <glm/glm.hpp>

namespace segfault::renderer {

    class RHI;

    enum class BufferUsage : uint32_t {
        TransferSourceBit = 0x00000001,
        TransferDestinationBit = 0x00000002,
        UniformTexelBufferBit = 0x00000004,
        StorageTexelBufferBit = 0x00000008,
        UniformBuffer = 0x00000010,
        IndexBuffer = 0x00000040,
        VertexBuffer = 0x00000080,
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

    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    /// @brief Represents a mesh with vertices and indices.
    struct Mesh {
        VertexArray vertices;  ///< Array of vertices in the mesh.
        IndexArray indices;    ///< Array of indices for indexed drawing.

        /// @brief Constructs a new mesh.
        Mesh() = default;

        /// @brief Destroys the mesh.
        ~Mesh() = default;

        /// @brief Sets the vertices for the mesh.
        /// @param verts The array of vertices to set.
        void setVertices(const VertexArray& verts) {
            vertices = verts;
        }

        /// @brief Sets the indices for the mesh.
        /// @param inds The array of indices to set.
        void setIndices(const IndexArray& inds) {
            indices = inds;
        }
    };


    struct Frame {
        void begin();
        void submit();
        void end();
    };

	inline void Frame::begin() {
		// Implementation for beginning a frame
	}

	inline void Frame::submit() {
		// Implementation for submitting a frame
	}

	inline void Frame::end() {
		// Implementation for ending a frame
	}

} // namespace segfault::renderer
