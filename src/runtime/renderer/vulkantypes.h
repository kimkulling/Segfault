#pragma once

#include <cstdint>

namespace segfault::renderer {

    enum class BufferUsage : uint32_t {
        VertexBuffer = 0x1,
        IndexBuffer = 0x2,
        UniformBuffer = 0x4
    };

} // namespace segfault::renderer
