#pragma once

#include "core/segfault.h"

struct SDL_Window;

namespace segfault::renderer {

    struct RHIImpl;

    //---------------------------------------------------------------------------------------------
    /// @class RHI
	/// @brief RHI (Rendering Hardware Interface) class provides an abstraction layer for rendering operations.
    /// 
    /// Allowing for flexibility in choosing different graphics APIs (e.g., Vulkan, DirectX, OpenGL) 
    /// without changing the higher-level rendering code. It manages the initialization, rendering, 
    /// and cleanup of graphics resources.
    //---------------------------------------------------------------------------------------------
    class RHI {
    public:
        /// @brief Constructs a new RHI instance.
        RHI();
        
        /// @brief Destroys the RHI instance.
        ~RHI();
        
        /// @brief Initializes the RHI with the specified application name and window.
        /// @param[ in ] appName The name of the application.
        /// @param[ in ] window The SDL window to use for rendering.
        /// @return True if initialization was successful, false otherwise.
        bool init(const char* appName, SDL_Window* window);
        
        /// @brief Shuts down the RHI.
        /// @return True if shutdown was successful, false otherwise.
        bool shutdown();
        
        /// @brief Draws a single frame.
        void drawFrame();
		
        /// @brief Resizes the rendering surface.
        void resize();

    private:
        RHIImpl* mImpl{ nullptr };
    };

} // namespace segfault::renderer
