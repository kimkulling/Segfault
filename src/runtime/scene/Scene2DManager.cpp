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

#include "scene/Scene2DManager.h"
#include "scene/Scene2D.h"
#include "renderer/RHI.h"

#include <algorithm>

namespace segfault::scene {

    //---------------------------------------------------------------------------------------------
    // Scene2DManager implementation
    //---------------------------------------------------------------------------------------------

    Scene2DManager::Scene2DManager() : mActiveScene(nullptr), mRHI(nullptr) {
        // empty
    }

    Scene2DManager::~Scene2DManager() {
        shutdown();
    }

    bool Scene2DManager::init(renderer::RHI& rhi) {
        if (mRHI != nullptr) {
            return true; // Already initialized
        }

        mRHI = &rhi;
        mActiveScene = nullptr;
        mScenes.clear();
        mRenderOrder.clear();

        return true;
    }

    bool Scene2DManager::shutdown() {
        unloadAllScenes();
        mRHI = nullptr;
        mActiveScene = nullptr;
        mSceneFactories.clear();
        return true;
    }

    Scene2D* Scene2DManager::createScene(const std::string& name, const std::string& sceneClassName) {
        if (mScenes.find(name) != mScenes.end()) {
            core::logMessage(core::LogType::Warn, ("Scene with name '" + name + "' already exists").c_str());
            return nullptr;
        }

        auto factoryIt = mSceneFactories.find(sceneClassName);
        if (factoryIt == mSceneFactories.end()) {
            core::logMessage(core::LogType::Error, ("Scene factory for class '" + sceneClassName + "' not registered").c_str());
            return nullptr;
        }

        Scene2D* newScene = factoryIt->second();
        if (newScene == nullptr) {
            core::logMessage(core::LogType::Error, ("Failed to create scene of class '" + sceneClassName + "'").c_str());
            return nullptr;
        }

        newScene->mName = name;
        newScene->mManager = this;

        if (!newScene->init(*mRHI)) {
            core::logMessage(core::LogType::Error, ("Failed to initialize scene '" + name + "'").c_str());
            delete newScene;
            return nullptr;
        }

        auto result = mScenes.emplace(name, std::unique_ptr<Scene2D>(newScene));
        if (!result.second) {
            core::logMessage(core::LogType::Error, ("Failed to emplace scene '" + name + "' into manager").c_str());
            delete newScene;
            return nullptr;
        }

        // If this is the first scene, activate it
        if (mActiveScene == nullptr) {
            mActiveScene = newScene;
        }

        // Add to render order if not already present
        if (std::find(mRenderOrder.begin(), mRenderOrder.end(), name) == mRenderOrder.end()) {
            mRenderOrder.push_back(name);
        }

        core::logMessage(core::LogType::Info, ("Created scene '" + name + "' of class '" + sceneClassName + "'").c_str());

        return newScene;
    }

    Scene2D* Scene2DManager::loadScene(const std::string& name, std::function<Scene2D*(void)> factory) {
        if (mScenes.find(name) != mScenes.end()) {
            core::logMessage(core::LogType::Warn, ("Scene with name '" + name + "' already exists").c_str());
            return nullptr;
        }

        if (factory == nullptr) {
            core::logMessage(core::LogType::Error, "Scene factory function is null");
            return nullptr;
        }

        Scene2D* newScene = factory();
        if (newScene == nullptr) {
            core::logMessage(core::LogType::Error, ("Factory failed to create scene '" + name + "'").c_str());
            return nullptr;
        }

        newScene->mName = name;
        newScene->mManager = this;

        if (!newScene->init(*mRHI)) {
            core::logMessage(core::LogType::Error, ("Failed to initialize scene '" + name + "'").c_str());
            delete newScene;
            return nullptr;
        }

        auto result = mScenes.emplace(name, std::unique_ptr<Scene2D>(newScene));
        if (!result.second) {
            core::logMessage(core::LogType::Error, ("Failed to emplace scene '" + name + "' into manager").c_str());
            delete newScene;
            return nullptr;
        }

        // If this is the first scene, activate it
        if (mActiveScene == nullptr) {
            mActiveScene = newScene;
        }

        // Add to render order if not already present
        if (std::find(mRenderOrder.begin(), mRenderOrder.end(), name) == mRenderOrder.end()) {
            mRenderOrder.push_back(name);
        }

        core::logMessage(core::LogType::Info, ("Loaded scene '" + name + "'").c_str());

        return newScene;
    }

    bool Scene2DManager::unloadScene(const std::string& name) {
        auto it = mScenes.find(name);
        if (it == mScenes.end()) {
            core::logMessage(core::LogType::Warn, ("Scene '" + name + "' not found for unloading").c_str());
            return false;
        }

        // Check if this scene is active
        if (mActiveScene == it->second.get()) {
            mActiveScene = nullptr;
        }

        // Remove from render order
        auto renderIt = std::find(mRenderOrder.begin(), mRenderOrder.end(), name);
        if (renderIt != mRenderOrder.end()) {
            mRenderOrder.erase(renderIt);
        }

        // Shutdown and remove the scene
        it->second->shutdown();
        mScenes.erase(it);

        core::logMessage(core::LogType::Info, ("Unloaded scene '" + name + "'").c_str());

        return true;
    }

    void Scene2DManager::unloadAllScenes() {
        // Deactivate current scene first
        mActiveScene = nullptr;

        // Shutdown and clear all scenes
        for (auto& pair : mScenes) {
            pair.second->shutdown();
        }
        mScenes.clear();
        mRenderOrder.clear();

        core::logMessage(core::LogType::Info, "Unloaded all scenes");
    }

    bool Scene2DManager::activateScene(const std::string& name) {
        auto it = mScenes.find(name);
        if (it == mScenes.end()) {
            core::logMessage(core::LogType::Error, ("Scene '" + name + "' not found for activation").c_str());
            return false;
        }

        if (mActiveScene != nullptr) {
            mActiveScene->onDeactivate();
        }

        mActiveScene = it->second.get();
        mActiveScene->onActivate();

        core::logMessage(core::LogType::Info, ("Activated scene '" + name + "'").c_str());

        return true;
    }

    void Scene2DManager::deactivateCurrentScene() {
        if (mActiveScene != nullptr) {
            mActiveScene->onDeactivate();
            mActiveScene = nullptr;
            core::logMessage(core::LogType::Info, "Deactivated current scene");
        }
    }

    Scene2D* Scene2DManager::getActiveScene() const {
        return mActiveScene;
    }

    Scene2D* Scene2DManager::getScene(const std::string& name) const {
        auto it = mScenes.find(name);
        if (it == mScenes.end()) {
            return nullptr;
        }
        return it->second.get();
    }

    bool Scene2DManager::hasScene(const std::string& name) const {
        return mScenes.find(name) != mScenes.end();
    }

    void Scene2DManager::update(float deltaTime) {
        // Update all loaded scenes
        for (auto& pair : mScenes) {
            pair.second->update(deltaTime);
        }
    }

    void Scene2DManager::render(renderer::RHI& rhi) {
        if (mActiveScene != nullptr) {
            mActiveScene->render(rhi);
        }
    }

    void Scene2DManager::renderScene(renderer::RHI& rhi, const std::string& name) {
        auto it = mScenes.find(name);
        if (it != mScenes.end()) {
            it->second->render(rhi);
        }
    }

    void Scene2DManager::setRenderOrder(const std::vector<std::string>& sceneNames) {
        mRenderOrder = sceneNames;

        // Verify all scenes in render order exist
        for (const auto& name : mRenderOrder) {
            if (mScenes.find(name) == mScenes.end()) {
                core::logMessage(core::LogType::Warn, ("Scene '" + name + "' in render order does not exist").c_str());
            }
        }
    }

    std::vector<std::string> Scene2DManager::getSceneNames() const {
        std::vector<std::string> names;
        for (const auto& pair : mScenes) {
            names.push_back(pair.first);
        }
        return names;
    }

    uint32_t Scene2DManager::getSceneCount() const {
        return static_cast<uint32_t>(mScenes.size());
    }

    void Scene2DManager::registerSceneFactory(const std::string& className, std::function<Scene2D*(void)> factory) {
        mSceneFactories[className] = factory;
        core::logMessage(core::LogType::Info, ("Registered scene factory for class '" + className + "'").c_str());
    }

} // namespace segfault::scene
