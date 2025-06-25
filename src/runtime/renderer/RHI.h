#pragma once

#include "core/segfault.h"

namespace segfault::renderer {

    class RHI {
    public:
        RHI();
        ~RHI();
        void init();
        void shutdown();
        void beginFrame();
        void endFrame();
        void submitCommandBuffer();
    };

} // namespace segfault::renderer