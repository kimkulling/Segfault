#include "renderer/renderthread.h"

namespace segfault::renderer {

    RenderThread::RenderThread() {
        // Constructor implementation
    }
        
    RenderThread::~RenderThread() {
        // Destructor implementation
        if (mThread.joinable()) {
            mThread.join();
        }
    }
        
    void RenderThread::start() {
        mThread = std::thread(&RenderThread::run, this);
    }

    void RenderThread::stop() {
        if (mThread.joinable()) {
            mThread.join();
        }
    }
        
    bool RenderThread::isRunning() const {
        return mThread.joinable();
    }
        
    void RenderThread::run() {
        // Thread execution logic
        while (isRunning()) {
            // Perform rendering tasks
        }
    }


    void RenderThread::waitForCompletion() {
        if (mThread.joinable()) {
            mThread.join();
        }
    }
        
    void RenderThread::join() {
        if (mThread.joinable()) {
            mThread.join();
        }
    }

    void RenderThread::detach() {
        if (mThread.joinable()) {
            mThread.detach();
        }
    }

    bool RenderThread::isJoinable() const {
        return mThread.joinable();
    }

    bool RenderThread::isDetached() const {
        return !mThread.joinable() && mThread.get_id() != std::thread::id();
    }

    void RenderThread::setThreadName(const char* name) {
    }

    const char* RenderThread::getThreadName() const {
        return "RenderThread"; // Placeholder, as setting thread names is platform-specific
    }
        
    std::thread::id RenderThread::getThreadId() const  {
        return mThread.get_id();
    }

} // namespace segfault::renderer
