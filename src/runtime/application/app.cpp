#include "application/app.h"
#include "core/segfault.h"
#include <SDL.h>
#include <SDL_vulkan.h>
#include <volk.h>

#include <iostream>
#include <sstream>

namespace segfault::application {
    
    using namespace segfault::core;
    using namespace segfault::renderer;
    
    namespace {
        std::string getStartLog() {
            std::string entry = "===========================================================================\n";
            entry.append("= SegFault version 0.0.l                                                  =\n");
            entry.append("===========================================================================");
            return entry;
        }

        std::string getEndLog() {
            std::string entry = "===========================================================================\n";
            entry.append("= SegFault run finished ...                                               =\n");
            entry.append("===========================================================================");
            return entry;
        }

        std::string getSDLVersionString() {
            SDL_version compiled{};
            SDL_VERSION(&compiled);

            std::ostringstream oss;
            oss << static_cast<int>(compiled.major) << "."
                << static_cast<int>(compiled.minor) << "."
                << static_cast<int>(compiled.patch);
            return oss.str();
        }

        bool initSDL() {
            if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) == -1) {
                logMessage(LogType::Error, "Cannot init sdl.");
                return false;
            }

            if (SDL_Vulkan_LoadLibrary(nullptr) == -1) {
                logMessage(LogType::Error, "Cannot load sdl-vulkan libs.");
                return false;
            }

            const auto v = std::string("SDL version ") + getSDLVersionString() + std::string(" initiated.");            
            logMessage(LogType::Info, v.c_str());
            
            return true;
        }

        SDL_Window* initWindow(const char* title, uint32_t x, uint32_t y, uint32_t width, uint32_t height, bool fullscreen) {
            SDL_Window *sdlWindow = SDL_CreateWindow(title, x, y, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
            if (sdlWindow == nullptr) {
                std::string msg = std::string("Cannot init sdl-window: ");
                msg += SDL_GetError();
                logMessage(LogType::Error, msg.c_str());
                return nullptr;
            }

            return sdlWindow;
        }
    }

    App::App() : mState(ModuleState::Invalid), mSdlWindow(nullptr) {
        // empty
    }

    App::~App() {
        if (mState != ModuleState::Shutdown) {
            logMessage(LogType::Error, "App not shutdowned.");
        }
    }

    bool App::init(const char *appName, uint32_t x, uint32_t y, uint32_t width, uint32_t height, const char *title, bool fullscreen) {
        logMessage(LogType::Print, getStartLog().c_str());
        if (!initSDL()) {
            return false;
        }

        mState = ModuleState::Init;
        mSdlWindow = initWindow(title, x, y, width, height, fullscreen);
        if (mSdlWindow == nullptr) {
            return false;
        }

        mRHI = new RHI;
        mRHI->init(appName, mSdlWindow);

        return true;
    }

    bool App::mainloop() {
        bool running{ true };
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_WINDOWEVENT_RESIZED:
                    onResize();
                    break;
            }
        }
            
        return running;
    }

    void App::drawFrame() {
        mRHI->drawFrame();
    }

    void App::onResize() {
        mRHI->resize();
    }
    
    void App::shutdown() {
        if (mSdlWindow == nullptr) {
            logMessage(LogType::Error, "Invalid application state, cannot shutdown.");
            return;
        }
        
        SDL_DestroyWindow(mSdlWindow);
        mSdlWindow = nullptr;
        mState = ModuleState::Shutdown;
        SDL_Quit();
        logMessage(LogType::Print, getEndLog().c_str());
    }

} // namespace segfault::application
