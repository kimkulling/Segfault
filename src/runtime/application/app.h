#pragma once

#include "core/segfault.h"
#include "renderer/renderthread.h"
#include "renderer/RHI.h"

struct SDL_Window;

namespace segfault::application {

    class SEGFAULT_EXPORT App final {
    public:
        using Rect = core::Rect;
        
        // No copying
        App(const App &rhs) = delete;
        App& operator = (const App& rhs) = delete;

        App();
        ~App();
        bool init(const char* appName, const Rect &rect, const char* title, bool fullscreen);
        bool mainloop();
        void shutdown();
        void drawFrame();

    private:
        void onResize();

    private:
        core::ModuleState mState;
        renderer::RenderThread mRenderThread;
        SDL_Window *mSdlWindow = nullptr;
        renderer::RHI *mRHI = nullptr;
    };

} // namespace segfault::application
