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
