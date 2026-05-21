#pragma once

#include "core/segfault.h"

#include <glm/glm.hpp>

namespace segfault::renderer {

    class RHI;

    struct RenderBuffer {
        core::guid id = 0;
        uint32_t size = 0;
        uint32_t stride = 0;
        uint32_t offset = 0;
    };

    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    class RenderGraph {
    public:
        RenderGraph() = default;

        ~RenderGraph() = default;

        void execute(RHI &rhi) {
            // Implementation for executing the render graph
        }
    };

} // namespace segfault::renderer
