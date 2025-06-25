#include "application/app.h"
#include <SDL.h>

#include <iostream>

namespace segfault::application {

    enum LogType {
        Invalid = -1,
        Error,
        Warn,
        Info,
        Count
    };

    void logMessage(LogType type, const char* msg) {
        switch (type) {
            case Error:
                std::cout << "*Err*  : " << msg << std::endl;
                break;
            case Warn:
                std::cout << "*Warn* : " << msg << std::endl;
                break;
            case Info:
                std::cout << "*Info* : " << msg << std::endl;
                break;
        }
        std::cout << msg << std::endl;
    }

    App::App() : mState(Invalid), mSDL_Window(nullptr) {

    }

    App::~App() {
        if (mState != Shutdown) {
            logMessage(Error, "App not shutdowned.");
        }
    }

    bool App::init(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const char *title, bool fullscreen) {
        if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) == -1) {
            logMessage(Error, "Cannot init sdl.");
            return false;
        }
        mState = Init;
        mSDL_Window = SDL_CreateWindow(title, x, y, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (mSDL_Window == nullptr) {
            logMessage(Error, "Cannot init window.");
            return false;
        }

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
        SDL_DestroyWindow(mSDL_Window);
        mSDL_Window = nullptr;
        mState = Shutdown;
        SDL_Quit();
    }

} // namespace segfault::application
