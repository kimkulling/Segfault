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
        void drawFrame();

    private:
        RHIImpl *mImpl;
    };

} // namespace segfault::renderer
