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
#include "application/app.h"

namespace segfault::examples {

    //---------------------------------------------------------------------------------------------
    /// @struct ExampleConfig
    /// @brief Describes the window and naming setup for an example application.
    //---------------------------------------------------------------------------------------------
    struct ExampleConfig {
        const char *appName{"segfault_example"};   ///< Internal application name.
        const char *title{"Segfault Example"};     ///< Window title shown to the user.
        uint32_t x{50};                             ///< Window origin x position.
        uint32_t y{50};                             ///< Window origin y position.
        uint32_t width{800};                        ///< Window width in pixels.
        uint32_t height{600};                       ///< Window height in pixels.
        bool fullscreen{false};                     ///< Whether to start in fullscreen mode.
    };

    //---------------------------------------------------------------------------------------------
    /// @class ExampleBase
    /// @brief Base class for all Segfault examples.
    ///
    /// ExampleBase removes the boilerplate of window creation and the main loop so that each
    /// example only needs to implement the parts that are interesting for the subsystem it
    /// demonstrates. It owns an application::App instance (App is final and therefore composed,
    /// not inherited) and drives the standard init -> loop -> shutdown lifecycle. Derived classes
    /// override the onXXX hooks to inject their own behaviour.
    ///
    /// Typical usage:
    /// @code
    /// class MyExample final : public segfault::examples::ExampleBase {
    /// public:
    ///     MyExample() : ExampleBase({"my_example", "My Example", 50, 50, 1280, 720, false}) {}
    ///
    /// protected:
    ///     bool onSetup() override { return true; }
    ///     void onUpdate(float dt) override {}
    ///     void onShutdown() override {}
    /// };
    ///
    /// int main(int argc, char *argv[]) {
    ///     MyExample example;
    ///     return example.run(argc, argv);
    /// }
    /// @endcode
    //---------------------------------------------------------------------------------------------
    class ExampleBase {
    public:
        // No copying.
        ExampleBase(const ExampleBase &rhs) = delete;
        ExampleBase &operator=(const ExampleBase &rhs) = delete;

        /// @brief Constructs the example with the given configuration.
        /// @param[ in ] config The window and naming configuration for this example.
        explicit ExampleBase(const ExampleConfig &config);

        /// @brief The class destructor.
        virtual ~ExampleBase();

        /// @brief Runs the complete example lifecycle: init, main loop and shutdown.
        /// @param[ in ] argc The argument count forwarded from main.
        /// @param[ in ] argv The argument vector forwarded from main.
        /// @return Zero on success, a negative value if initialization failed.
        int run(int argc, char *argv[]);

        /// @brief Returns the configuration used by this example.
        const ExampleConfig &getConfig() const;

    protected:
        /// @brief Called once after the application has been initialized.
        /// @return True to continue running, false to abort startup.
        virtual bool onSetup();

        /// @brief Called once per frame before the frame is drawn.
        /// @param[ in ] dt The elapsed time since the previous frame in seconds.
        virtual void onUpdate(float dt);

        /// @brief Called once before the application is shut down.
        virtual void onShutdown();

        /// @brief Grants derived classes access to the underlying application instance.
        application::App &getApp();

    private:
        ExampleConfig mConfig;
        application::App mApp;
        core::ModuleState mState;
    };

} // namespace segfault::examples
