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
#include "examplebase.h"

#include <chrono>

namespace segfault::examples {

using core::LogType;
using core::ModuleState;
using core::Rect;

ExampleBase::ExampleBase(const ExampleConfig &config) :
        mConfig(config), mApp(), mState(ModuleState::Invalid) {
    // empty
}

ExampleBase::~ExampleBase() {
    // empty
}

int ExampleBase::run(int, char *[]) {
    const Rect rect(mConfig.x, mConfig.y, mConfig.width, mConfig.height);
    if (!mApp.init(mConfig.appName, rect, mConfig.title, mConfig.fullscreen)) {
        core::logMessage(LogType::Error, "Failed to initialize the application.");
        return -1;
    }

    mState = ModuleState::Init;
    if (!onSetup()) {
        core::logMessage(LogType::Error, "Example setup failed.");
        mApp.shutdown();
        return -1;
    }

    mState = ModuleState::Running;
    auto last = std::chrono::steady_clock::now();
    while (mApp.mainloop()) {
        const auto now = std::chrono::steady_clock::now();
        const float dt = std::chrono::duration<float>(now - last).count();
        last = now;

        onUpdate(dt);
        mApp.drawFrame();
    }

    mState = ModuleState::Shutdown;
    onShutdown();
    mApp.shutdown();

    return 0;
}

const ExampleConfig &ExampleBase::getConfig() const {
    return mConfig;
}

bool ExampleBase::onSetup() {
    return true;
}

void ExampleBase::onUpdate(float) {
    // empty
}

void ExampleBase::onShutdown() {
    // empty
}

application::App &ExampleBase::getApp() {
    return mApp;
}

} // namespace segfault::examples
