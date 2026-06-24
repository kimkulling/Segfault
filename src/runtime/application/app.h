/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2026 Segfault by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#pragma once

#include "core/segfault.h"
#include "renderer/renderthread.h"
#include "renderer/RHI.h"

struct SDL_Window;

namespace segfault::application {

    //---------------------------------------------------------------------------------------------
    /// @class App
	/// @brief The App class serves as the main entry point for the application.
    /// 
    /// Managing the lifecycle of the application, including initialization, main loop, and 
    /// shutdown. It encapsulates the core functionality of the application, such as window 
    /// management, rendering, and event handling.
    //---------------------------------------------------------------------------------------------
    class SEGFAULT_EXPORT App final {
    public:
        using Rect = core::Rect;
        
        // No copying
        App(const App &rhs) = delete;
        App& operator = (const App& rhs) = delete;

        ///  @brief The class constructor.
        App();

		/// @brief The class destructor.
        ~App();

		/// @brief Initializes the application with the specified parameters.
		/// @param[ in ] appName The name of the application.
		/// @param[ in ] rect The dimensions of the application window.
		/// @param[ in ] title The title of the application window.
		/// @param[ in ] fullscreen Whether to start the application in fullscreen mode.
		/// @return True if initialization was successful, false otherwise.
        bool init(const char* appName, const Rect &rect, const char* title,bool fullscreen);

		/// @brief The main loop of the application, which processes events and renders frames.
		/// @return True if the main loop should continue running, false if it should exit.
        bool mainloop();

		/// @brief Shuts down the application and releases all resources.
        void shutdown();

		/// @brief Draws a single frame of the application.
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
