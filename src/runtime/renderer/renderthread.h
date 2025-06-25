#pragma once

#include "core/segfault.h"

#include <thread>

namespace segfault::renderer {

    class RenderThread {
    public:
        RenderThread();
        ~RenderThread();
        void start();
        void stop();
        bool isRunning() const;
        void run();
        void waitForCompletion();
        void join();
        void detach();
        bool isJoinable() const;
        bool isDetached() const;
        void setThreadName(const char* name);
        const char* getThreadName() const;
        std::thread::id getThreadId() const;

    private:
        std::thread mThread;
    };

} // namespace segfault::renderer