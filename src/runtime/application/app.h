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
