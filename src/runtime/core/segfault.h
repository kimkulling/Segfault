#pragma once

#include "core/config.h"

#include <cstdint>
#include <iostream>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#    define SEGFAULT_WINDOWS
#    define _CRT_SECURE_NO_WARNINGS
#    ifndef WIN32_LEAN_AND_MEAN
#        define WIN32_LEAN_AND_MEAN // Minimal windows header
#    endif // WIN32_LEAN_AND_MEAN
#elif defined(__gnu_linux__)
#    define SEGFAULT_GNU_LINUX
#elif defined(__APPLE__) || defined(__MACH__)
#    error "Currently not supported platform"
#elif defined(__ANDROID__)
#    define SEGFAULT_ANDROID
#endif

#ifdef SEGFAULT_WINDOWS
#    define SEGFAULT_TAG_DLL_EXPORT __declspec(dllexport)
#    define SEGFAULT_TAG_DLL_IMPORT __declspec(dllimport)
#endif
#ifdef SEGFAULT_WINDOWS
#    ifdef SEGFAULT_BUILD_EXPORT
#        define SEGFAULT_EXPORT SEGFAULT_TAG_DLL_EXPORT
#    else
#        define SEGFAULT_EXPORT SEGFAULT_TAG_DLL_IMPORT
#    endif
#else
#    define SEGFAULT_EXPORT __attribute__((visibility("default")))
#endif

namespace segfault::core {

    using guid = uint64_t;    

    enum class ModuleState {
        Invalid = -1,
        Init,
        Running,
        Shutdown,
        Count
    };

    enum class LogType {
        Invalid = -1,
        Error,
        Warn,
        Info,
        Count
    };

    inline void logMessage(LogType type, const char* msg) {
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

} // namespace segfault::core
