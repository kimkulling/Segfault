#pragma once

#include "core/segfault.h"
#include "renderer/renderthread.h"

namespace segfault::application {

    class SEGFAULT_EXPORT App {
    public:
        App();
        ~App();

        bool init(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const char* title, bool fullscreen);
        void run();
        void shutdown();

    private:
        enum State {
            Invalid = -1,
            Init,
            Running,
            Shutdown,
            Count
        };
        renderer::RenderThread mRenderThread;
    };

} // namespace segfault::application
