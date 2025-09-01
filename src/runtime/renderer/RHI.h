#pragma once

#include "core/segfault.h"

struct SDL_Window;

namespace segfault::renderer {

    struct RHIImpl;

    class RHI {
    public:
        RHI();
        ~RHI();
        bool init(const char* appName, SDL_Window* window);
        bool shutdown();
        void beginFrame();
        void drawFrame();
        void endFrame();
        void submitCommandBuffer();

    private:
        RHIImpl *mImpl;
    };

} // namespace segfault::renderer