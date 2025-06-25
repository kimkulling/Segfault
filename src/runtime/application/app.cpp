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

    App::App() {

    }

    App::~App() {

    }

    bool App::init(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const char *title, bool fullscreen) {
        if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS) == -1) {
            logMessage(Error, "Cannot init windows.");
            return false;
        }

        return true;
    }

    void App::run() {

    }
    
    void App::shutdown() {
        SDL_Quit();
    }

} // namespace segfault::application
