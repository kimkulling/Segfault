#pragma once

#include <cstdint>
#include <optional>

namespace segfault::renderer {

    enum class BufferUsage : uint32_t {
        VertexBuffer = 0x1,
        IndexBuffer = 0x2,
        UniformBuffer = 0x4
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


} // namespace segfault::renderer
