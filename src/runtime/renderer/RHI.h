#pragma once

#include "core/segfault.h"

struct SDL_Window;

namespace segfault::renderer {

    struct RHIImpl;

    class RHI {
    public:
        RHI();
        ~RHI();
        bool init(SDL_Window* window);
        bool shutdown();
        void beginFrame();
        void endFrame();
        void submitCommandBuffer();

    private:
        RHIImpl *mImpl;
    };

} // namespace segfault::renderer