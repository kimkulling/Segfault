#pragma once

#include "core/segfault.h"

namespace segfault::renderer {

    class RHI;

    struct RenderPipelineState {

    };

    struct RenderBuffer {
        core::guid id = 0;
        uint32_t size = 0;
        uint32_t stride = 0;
        uint32_t offset = 0;
    };

    struct DrawCommand {
        RenderPipelineState rpState;

        uint32_t numVertices = 0;
        uint32_t numInstances = 0;
        uint32_t vertexOffset = 0;  
        uint32_t instanceOffset = 0;
        uint32_t indexCount = 0;
    };

    class RenderGraph {
    public:
        RenderGraph() = default;
        ~RenderGraph() = default;

        void addCommand(const DrawCommand& command) {
            // Implementation for adding a draw command to the render graph
        }

        void execute(RHI &rhi) {
            // Implementation for executing the render graph
        }
    };

} // namespace segfault::renderer
