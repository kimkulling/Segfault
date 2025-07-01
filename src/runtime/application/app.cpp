#include "application/app.h"
#include "core/segfault.h"
#include <SDL.h>

#include <iostream>

namespace segfault::application {
    
    using namespace segfault::core;
    using namespace segfault::renderer;
    
    enum class LogType {
        Invalid = -1,
        Error,
        Warn,
        Info,
        Count
    };

    void logMessage(LogType type, const char* msg) {
        switch (type) {
            case LogType::Error:
                std::cout << "*Err*  : " << msg << std::endl;
                break;
            case LogType::Warn:
                std::cout << "*Warn* : " << msg << std::endl;
                break;
            case LogType::Info:
                std::cout << "*Info* : " << msg << std::endl;
                break;
        }
        std::cout << msg << std::endl;
    }

    App::App() : mState(ModuleState::Invalid), mSdlWindow(nullptr) {

    }

    App::~App() {
        if (mState != ModuleState::Shutdown) {
            logMessage(LogType::Error, "App not shutdowned.");
        }
    }

    bool App::init(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const char *title, bool fullscreen) {
        if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) == -1) {
            logMessage(LogType::Error, "Cannot init sdl.");
            return false;
        }
        mState = ModuleState::Init;
        mSdlWindow = SDL_CreateWindow(title, x, y, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (mSdlWindow == nullptr) {
            logMessage(LogType::Error, "Cannot init window.");
            return false;
        }


        mRHI = new RHI;
        mRHI->init(mSdlWindow);

        return true;

    }

    bool App::run() {
        bool running = true;
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
        }
            
        return running;
    }
    
    void App::shutdown() {
        SDL_DestroyWindow(mSdlWindow);
        mSdlWindow = nullptr;
        mState = ModuleState::Shutdown;
        SDL_Quit();
    }

} // namespace segfault::application
