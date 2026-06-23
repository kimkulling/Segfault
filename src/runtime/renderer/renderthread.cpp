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
