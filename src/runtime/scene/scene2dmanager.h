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
#include "scene/scene2d.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

namespace segfault::scene {

    // Forward declarations
    class Scene2D;

    //---------------------------------------------------------------------------------------------
    /// @class Scene2DManager
    /// @brief Manages multiple 2D scenes, providing loading, unloading, activation, and rendering services.
    ///
    /// The Scene2DManager is the central point for handling all 2D rendering scenes in the engine.
    /// It maintains a collection of scenes, manages scene transitions, and coordinates rendering.
    //---------------------------------------------------------------------------------------------
    class SEGFAULT_EXPORT Scene2DManager final {
    public:
        /// @brief Constructs a new Scene2DManager.
        Scene2DManager();

        /// @brief Destroys the Scene2DManager and all loaded scenes.
        ~Scene2DManager();

        /// @brief Deleted copy constructor.
        Scene2DManager(const Scene2DManager&) = delete;

        /// @brief Deleted copy assignment operator.
        Scene2DManager& operator=(const Scene2DManager&) = delete;

        //-----------------------------------------------------------------------------------------
        /// @brief Initializes the scene manager.
        /// @param[ in ] rhi The rendering hardware interface to use.
        /// @return True if initialization was successful, false otherwise.
        //-----------------------------------------------------------------------------------------
        bool init(renderer::RHI& rhi);

        //-----------------------------------------------------------------------------------------
        /// @brief Shuts down the scene manager and releases all resources.
        /// @return True if shutdown was successful, false otherwise.
        //-----------------------------------------------------------------------------------------
        bool shutdown();

        //-----------------------------------------------------------------------------------------
        /// @brief Creates and loads a new 2D scene.
        /// @param[ in ] name The unique name of the scene.
        /// @param[ in ] sceneClassName The class name of the scene to instantiate.
        /// @return Pointer to the created scene, or nullptr if creation failed.
        //-----------------------------------------------------------------------------------------
        Scene2D* createScene(const std::string& name, const std::string& sceneClassName);

        //-----------------------------------------------------------------------------------------
        /// @brief Loads a 2D scene from a custom factory function.
        /// @param[ in ] name The unique name of the scene.
        /// @param[ in ] factory Function that creates and returns a Scene2D pointer.
        /// @return Pointer to the loaded scene, or nullptr if loading failed.
        //-----------------------------------------------------------------------------------------
        Scene2D* loadScene(const std::string& name, std::function<Scene2D*(void)> factory);

        //-----------------------------------------------------------------------------------------
        /// @brief Unloads a scene by name.
        /// @param[ in ] name The name of the scene to unload.
        /// @return True if the scene was found and unloaded, false otherwise.
        //-----------------------------------------------------------------------------------------
        bool unloadScene(const std::string& name);

        //-----------------------------------------------------------------------------------------
        /// @brief Unloads all loaded scenes.
        //-----------------------------------------------------------------------------------------
        void unloadAllScenes();

        //-----------------------------------------------------------------------------------------
        /// @brief Activates a scene for rendering and updates.
        /// @param[ in ] name The name of the scene to activate.
        /// @return True if the scene was found and activated, false otherwise.
        //-----------------------------------------------------------------------------------------
        bool activateScene(const std::string& name);

        //-----------------------------------------------------------------------------------------
        /// @brief Deactivates the currently active scene.
        //-----------------------------------------------------------------------------------------
        void deactivateCurrentScene();

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the currently active scene.
        /// @return Pointer to the active scene, or nullptr if no scene is active.
        //-----------------------------------------------------------------------------------------
        Scene2D* getActiveScene() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the currently active scene by name.
        /// @param[ in ] name The name of the scene to retrieve.
        /// @return Pointer to the scene, or nullptr if not found.
        //-----------------------------------------------------------------------------------------
        Scene2D* getScene(const std::string& name) const;

        //-----------------------------------------------------------------------------------------
        /// @brief Checks if a scene with the given name exists.
        /// @param[ in ] name The name of the scene to check.
        /// @return True if the scene exists, false otherwise.
        //-----------------------------------------------------------------------------------------
        bool hasScene(const std::string& name) const;

        //-----------------------------------------------------------------------------------------
        /// @brief Updates all loaded scenes.
        /// @param[ in ] deltaTime The time elapsed since the last frame in seconds.
        //-----------------------------------------------------------------------------------------
        void update(float deltaTime);

        //-----------------------------------------------------------------------------------------
        /// @brief Renders the active scene.
        /// @param[ in ] rhi The rendering hardware interface to use.
        //-----------------------------------------------------------------------------------------
        void render(renderer::RHI& rhi);

        //-----------------------------------------------------------------------------------------
        /// @brief Renders a specific scene by name.
        /// @param[ in ] rhi The rendering hardware interface to use.
        /// @param[ in ] name The name of the scene to render.
        //-----------------------------------------------------------------------------------------
        void renderScene(renderer::RHI& rhi, const std::string& name);

        //-----------------------------------------------------------------------------------------
        /// @brief Sets the rendering order for scenes.
        /// @param[ in ] sceneNames Array of scene names in the desired rendering order.
        //-----------------------------------------------------------------------------------------
        void setRenderOrder(const std::vector<std::string>& sceneNames);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets all loaded scene names.
        /// @return Array of all loaded scene names.
        //-----------------------------------------------------------------------------------------
        std::vector<std::string> getSceneNames() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the number of loaded scenes.
        /// @return The number of loaded scenes.
        //-----------------------------------------------------------------------------------------
        uint32_t getSceneCount() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Registers a scene factory function for a specific scene class name.
        /// @param[ in ] className The class name to register the factory for.
        /// @param[ in ] factory The factory function that creates scenes of this type.
        //-----------------------------------------------------------------------------------------
        void registerSceneFactory(const std::string& className, std::function<Scene2D*(void)> factory);

    private:
        using SceneMap = std::unordered_map<std::string, std::unique_ptr<Scene2D>>;
        using SceneFactoryMap = std::unordered_map<std::string, std::function<Scene2D*(void)>>;

        SceneMap mScenes;
        SceneFactoryMap mSceneFactories;
        Scene2D* mActiveScene{ nullptr };
        std::vector<std::string> mRenderOrder;
        renderer::RHI* mRHI{ nullptr };
    };

} // namespace segfault::scene
