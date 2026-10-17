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

#include "scene/scene2d.h"
#include "renderer/RHI.h"

#include <algorithm>

namespace segfault::scene {

    //---------------------------------------------------------------------------------------------
    // Camera2D implementation
    //---------------------------------------------------------------------------------------------

    Camera2D::Camera2D() : mZoom(1.0f), mRotation(0.0f), mViewMatrix(1.0f), mProjectionMatrix(1.0f), mDirty(true) {
        updateMatrices();
    }

    void Camera2D::setOrthographic(float left, float right, float bottom, float top, float near, float far) {
        mProjectionMatrix = glm::ortho(left, right, bottom, top, near, far);
        mDirty = true;
    }

    void Camera2D::setOrthographicFromViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height, float near, float far) {
        float halfWidth = static_cast<float>(width) * 0.5f;
        float halfHeight = static_cast<float>(height) * 0.5f;
        setOrthographic(-halfWidth, halfWidth, -halfHeight, halfHeight, near, far);
        mViewport.x = x;
        mViewport.y = y;
        mViewport.width = width;
        mViewport.height = height;
        mDirty = true;
    }

    void Camera2D::setPosition(const glm::vec2& position) {
        if (mPosition != position) {
            mPosition = position;
            mDirty = true;
        }
    }

    void Camera2D::setPosition(float x, float y) {
        setPosition(glm::vec2(x, y));
    }

    const glm::vec2& Camera2D::getPosition() const {
        return mPosition;
    }

    void Camera2D::setZoom(float zoom) {
        if (mZoom != zoom) {
            mZoom = std::max(0.001f, zoom); // Prevent zero or negative zoom
            mDirty = true;
        }
    }

    float Camera2D::getZoom() const {
        return mZoom;
    }

    void Camera2D::setRotation(float angle) {
        if (mRotation != angle) {
            mRotation = angle;
            mDirty = true;
        }
    }

    float Camera2D::getRotation() const {
        return mRotation;
    }

    const glm::mat4& Camera2D::getViewMatrix() const {
        if (mDirty) {
            const_cast<Camera2D*>(this)->updateMatrices();
        }
        return mViewMatrix;
    }

    const glm::mat4& Camera2D::getProjectionMatrix() const {
        return mProjectionMatrix;
    }

    glm::mat4 Camera2D::getViewProjectionMatrix() const {
        if (mDirty) {
            const_cast<Camera2D*>(this)->updateMatrices();
        }
        return mProjectionMatrix * mViewMatrix;
    }

    glm::vec2 Camera2D::screenToWorld(const glm::vec2& screenPos, uint32_t viewportWidth, uint32_t viewportHeight) const {
        // Convert screen coordinates to normalized device coordinates
        float ndcX = (2.0f * screenPos.x) / static_cast<float>(viewportWidth) - 1.0f;
        float ndcY = 1.0f - (2.0f * screenPos.y) / static_cast<float>(viewportHeight);

        // Create NDC position with z=0
        glm::vec4 ndcPos(ndcX, ndcY, 0.0f, 1.0f);

        // Apply inverse view-projection matrix
        glm::mat4 invVP = glm::inverse(getViewProjectionMatrix());
        glm::vec4 worldPos = invVP * ndcPos;

        return glm::vec2(worldPos.x, worldPos.y);
    }

    glm::vec2 Camera2D::worldToScreen(const glm::vec2& worldPos, uint32_t viewportWidth, uint32_t viewportHeight) const {
        // Apply view-projection matrix
        glm::vec4 clipPos = getViewProjectionMatrix() * glm::vec4(worldPos.x, worldPos.y, 0.0f, 1.0f);

        // Convert from clip space to screen space
        float screenX = ((clipPos.x / clipPos.w) + 1.0f) * 0.5f * static_cast<float>(viewportWidth);
        float screenY = (1.0f - (clipPos.y / clipPos.w)) * 0.5f * static_cast<float>(viewportHeight);

        return glm::vec2(screenX, screenY);
    }

    void Camera2D::move(const glm::vec2& offset) {
        mPosition += offset;
        mDirty = true;
    }

    void Camera2D::resize(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        setOrthographicFromViewport(x, y, width, height);
    }

    void Camera2D::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        mViewport.x = x;
        mViewport.y = y;
        mViewport.width = width;
        mViewport.height = height;
        resize(x, y, width, height);
    }

    const core::Rect& Camera2D::getViewport() const {
        return mViewport;
    }

    void Camera2D::updateMatrices() {
        if (!mDirty) return;

        // Build view matrix: translate by position, rotate, scale by zoom
        glm::mat4 transform = glm::mat4(1.0f);
        
        // Apply zoom (scale)
        transform = glm::scale(transform, glm::vec3(mZoom, mZoom, 1.0f));
        
        // Apply rotation
        if (mRotation != 0.0f) {
            transform = glm::rotate(transform, glm::radians(mRotation), glm::vec3(0.0f, 0.0f, 1.0f));
        }
        
        // Apply translation
        transform = glm::translate(transform, glm::vec3(-mPosition.x, -mPosition.y, 0.0f));

        mViewMatrix = transform;
        mDirty = false;
    }

    //---------------------------------------------------------------------------------------------
    // Layer2D implementation
    //---------------------------------------------------------------------------------------------

    Layer2D::Layer2D() : mVisible(true), mRenderOrder(0), mCamera(nullptr), mUseOwnCamera(false) {
        // empty
    }

    bool Layer2D::init(renderer::RHI& rhi) {
        // Base layer initialization
        return true;
    }

    void Layer2D::shutdown() {
        // Base layer shutdown
    }

    void Layer2D::update(float deltaTime) {
        // Base layer update
    }

    void Layer2D::render(renderer::RHI& rhi) {
        // Base layer render
    }

    void Layer2D::setName(const std::string& name) {
        mName = name;
    }

    const std::string& Layer2D::getName() const {
        return mName;
    }

    void Layer2D::setVisible(bool visible) {
        mVisible = visible;
    }

    bool Layer2D::isVisible() const {
        return mVisible;
    }

    void Layer2D::setRenderOrder(int32_t order) {
        mRenderOrder = order;
    }

    int32_t Layer2D::getRenderOrder() const {
        return mRenderOrder;
    }

    void Layer2D::setCamera(Camera2D* camera) {
        mCamera = camera;
    }

    Camera2D* Layer2D::getCamera() const {
        return mCamera;
    }

    void Layer2D::setUseOwnCamera(bool useOwnCamera) {
        mUseOwnCamera = useOwnCamera;
    }

    bool Layer2D::getUseOwnCamera() const {
        return mUseOwnCamera;
    }

    //---------------------------------------------------------------------------------------------
    // Entity2D implementation
    //---------------------------------------------------------------------------------------------

    bool Entity2D::isValid() const {
        return id != 0;
    }

    bool Entity2D::operator==(const Entity2D& other) const {
        return id == other.id;
    }

    bool Entity2D::operator!=(const Entity2D& other) const {
        return id != other.id;
    }

    //---------------------------------------------------------------------------------------------
    // Transform2DComponent implementation
    //---------------------------------------------------------------------------------------------

    void Transform2DComponent::updateWorldMatrix() {
        if (!mDirty) {
            return;
        }

        auto transform = glm::mat4(1.0f);
        
        // Translate to position
        transform = glm::translate(transform, glm::vec3(mPosition.x, mPosition.y, 0.0f));
        
        // Rotate around origin
        if (mRotation != 0.0f) {
            transform = glm::translate(transform, glm::vec3(mSize.x * mOrigin.x, mSize.y * mOrigin.y, 0.0f));
            transform = glm::rotate(transform, glm::radians(mRotation), glm::vec3(0.0f, 0.0f, 1.0f));
            transform = glm::translate(transform, glm::vec3(-mSize.x * mOrigin.x, -mSize.y * mOrigin.y, 0.0f));
        }
        
        // Scale
        transform = glm::scale(transform, glm::vec3(mScale.x, mScale.y, 1.0f));

        mWorldMatrix = transform;
        mDirty = false;
    }

    void Transform2DComponent::setPosition(const glm::vec2& position) {
        if (mPosition != position) {
            mPosition = position;
            mDirty = true;
        }
    }

    void Transform2DComponent::setRotation(float rotation) {
        if (mRotation != rotation) {
            mRotation = rotation;
            mDirty = true;
        }
    }

    void Transform2DComponent::setScale(const glm::vec2& scale) {
        if (mScale != scale) {
            mScale = scale;
            mDirty = true;
        }
    }

    void Transform2DComponent::setSize(const glm::vec2& size) {
        if (mSize != size) {
            mSize = size;
            mDirty = true;
        }
    }

    void Transform2DComponent::setOrigin(const glm::vec2& origin) {
        if (mOrigin != origin) {
            mOrigin = origin;
            mDirty = true;
        }
    }

    //---------------------------------------------------------------------------------------------
    // SpriteComponent implementation
    //---------------------------------------------------------------------------------------------

    SpriteComponent::SpriteComponent(core::guid textureHandle) : mTextureHandle(textureHandle) {
        // empty
    }

    void SpriteComponent::setTexture(core::guid textureHandle) {
        mTextureHandle = textureHandle;
    }

    void SpriteComponent::setColor(const glm::vec4& color) {
        mColor = color;
    }

    void SpriteComponent::setAlpha(float alpha) {
        mAlpha = glm::clamp(alpha, 0.0f, 1.0f);
    }

    void SpriteComponent::setTextureRect(const core::Rect& rect) {
        mTextureRect = rect;
    }

    void SpriteComponent::setFlip(bool flipX, bool flipY) {
        mFlipX = flipX;
        mFlipY = flipY;
    }

    void SpriteComponent::setVisible(bool visible) {
        mVisible = visible;
    }

    //---------------------------------------------------------------------------------------------
    // TextComponent implementation
    //---------------------------------------------------------------------------------------------

    TextComponent::TextComponent(const std::string& text, core::guid fontHandle) 
        : mText(text), mFontHandle(fontHandle), mDirty(true) {
        // empty
    }

    void TextComponent::setText(const std::string& text) {
        if (mText != text) {
            mText = text;
            mDirty = true;
        }
    }

    void TextComponent::setFont(core::guid fontHandle) {
        if (mFontHandle != fontHandle) {
            mFontHandle = fontHandle;
            mDirty = true;
        }
    }

    void TextComponent::setColor(const glm::vec4& color) {
        mColor = color;
    }

    void TextComponent::setFontSize(float fontSize) {
        if (mFontSize != fontSize) {
            mFontSize = fontSize;
            mDirty = true;
        }
    }

    void TextComponent::setHorizontalAlignment(TextAlignment alignment) {
        mHorizontalAlignment = alignment;
        mDirty = true;
    }

    void TextComponent::setVerticalAlignment(VerticalAlignment alignment) {
        mVerticalAlignment = alignment;
        mDirty = true;
    }

    void TextComponent::setVisible(bool visible) {
        mVisible = visible;
    }

    //---------------------------------------------------------------------------------------------
    // Render2DComponent implementation
    //---------------------------------------------------------------------------------------------

    void Render2DComponent::setLayer(Layer2D* layer) {
        mLayer = layer;
    }

    void Render2DComponent::setRenderOrder(int32_t order) {
        mRenderOrder = order;
    }

    void Render2DComponent::setVisible(bool visible) {
        mVisible = visible;
    }

    //---------------------------------------------------------------------------------------------
    // Animation2DComponent implementation
    //---------------------------------------------------------------------------------------------

    void Animation2DComponent::addFrame(const core::Rect& textureRect, float duration) {
        mFrames.push_back({ textureRect, duration });
        mDirty = true;
    }

    void Animation2DComponent::setFrames(const std::vector<Frame>& frames) {
        mFrames = frames;
        mDirty = true;
    }

    void Animation2DComponent::play(bool loop) {
        mPlaying = true;
        mLoop = loop;
    }

    void Animation2DComponent::stop() {
        mPlaying = false;
        mCurrentFrame = 0;
        mFrameTimer = 0.0f;
    }

    void Animation2DComponent::pause() {
        mPlaying = false;
    }

    void Animation2DComponent::resume() {
        mPlaying = true;
    }

    void Animation2DComponent::update(float deltaTime) {
        if (!mPlaying || mFrames.empty()) return;

        mFrameTimer += deltaTime * mAnimationSpeed;
        
        const Frame& currentFrame = mFrames[mCurrentFrame];

        if (mCurrentFrame >= mFrames.size()) {
            mCurrentFrame = 0;
        }

        while (mFrames[mCurrentFrame].mDuration > 0.0f && mFrameTimer >= mFrames[mCurrentFrame].mDuration) {
            mFrameTimer -= mFrames[mCurrentFrame].mDuration;
            mCurrentFrame++;
            
            if (mCurrentFrame >= mFrames.size()) {
                if (mLoop) {
                    mCurrentFrame = 0;
                } else {
                    mPlaying = false;
                    mCurrentFrame = mFrames.size() - 1;
                    break;
                }
            }
        }
        
        mDirty = true;
    }

    core::Rect Animation2DComponent::getCurrentFrameRect() const {
        if (mFrames.empty()) return core::Rect();
        return mFrames[mCurrentFrame].mTextureRect;
    }

    uint32_t Animation2DComponent::getCurrentFrameIndex() const {
        return mCurrentFrame;
    }

    void Animation2DComponent::setCurrentFrameIndex(uint32_t frameIndex) {
        if (frameIndex < mFrames.size()) {
            mCurrentFrame = frameIndex;
            mFrameTimer = 0.0f;
            mDirty = true;
        }
    }

    void Animation2DComponent::setAnimationSpeed(float speed) {
        mAnimationSpeed = std::max(0.0f, speed);
    }

    bool Animation2DComponent::isPlaying() const {
        return mPlaying;
    }

    bool Animation2DComponent::isLooping() const {
        return mLoop;
    }

    uint32_t Animation2DComponent::getFrameCount() const {
        return static_cast<uint32_t>(mFrames.size());
    }

    //---------------------------------------------------------------------------------------------
    // Scene2D implementation
    //---------------------------------------------------------------------------------------------

    Scene2D::Scene2D() : mViewportWidth(0), mViewportHeight(0), mManager(nullptr) {
        mName = "UnnamedScene";
        mDefaultLayer = std::make_unique<Layer2D>();
        mDefaultLayer->setName("Default");
        mDefaultLayer->init(*static_cast<renderer::RHI*>(nullptr)); // Will be properly initialized later
    }

    Scene2D::~Scene2D() {
        shutdown();
    }

    bool Scene2D::init(renderer::RHI& rhi) {
        // Initialize main camera
        mMainCamera.resize(0, 0, mViewportWidth, mViewportHeight);
        
        // Initialize default layer
        if (mDefaultLayer) {
            mDefaultLayer->init(rhi);
        }
        
        // Initialize all other layers
        for (auto& pair : mLayers) {
            pair.second->init(rhi);
        }
        
        onLoad();
        return true;
    }

    void Scene2D::shutdown() {
        onUnload();
        
        // Shutdown all layers
        for (auto& pair : mLayers) {
            pair.second->shutdown();
        }
        mLayers.clear();
        
        if (mDefaultLayer) {
            mDefaultLayer->shutdown();
        }
    }

    void Scene2D::update(float deltaTime) {
        // Update all components that need updating
        for (auto& component : mAnimationPool.data()) {
            if (component.isPlaying()) {
                component.update(deltaTime);
            }
        }
        
        // Update all layers
        for (auto& pair : mLayers) {
            if (pair.second->isVisible()) {
                pair.second->update(deltaTime);
            }
        }
    }

    void Scene2D::render(renderer::RHI& rhi) {
        // Update transform matrices for all entities
        for (auto& component : mTransformPool.data()) {
            if (component.mDirty) {
                component.updateWorldMatrix();
            }
        }
        
        // Render all layers in order
        std::vector<Layer2D*> renderOrder;
        
        // Collect all layers
        for (auto& pair : mLayers) {
            renderOrder.push_back(pair.second.get());
        }
        
        // Sort by render order
        std::sort(renderOrder.begin(), renderOrder.end(), [](Layer2D* a, Layer2D* b) {
            return a->getRenderOrder() < b->getRenderOrder();
        });
        
        // Render default layer first
        if (mDefaultLayer && mDefaultLayer->isVisible()) {
            mDefaultLayer->render(rhi);
        }
        
        // Render sorted layers
        for (Layer2D* layer : renderOrder) {
            if (layer->isVisible()) {
                layer->render(rhi);
            }
        }
    }

    void Scene2D::onActivate() {
        // Override in derived classes
    }

    void Scene2D::onDeactivate() {
        // Override in derived classes
    }

    void Scene2D::onLoad() {
        // Override in derived classes
    }

    void Scene2D::onUnload() {
        // Override in derived classes
    }

    Entity2D Scene2D::createEntity() {
        Entity2D entity;
        entity.id = mNextEntityId++;
        return entity;
    }

    void Scene2D::destroyEntity(Entity2D entity) {
        if (!isValid(entity)) return;
        
        mTransformPool.remove(entity);
        mSpritePool.remove(entity);
        mTextPool.remove(entity);
        mRenderPool.remove(entity);
        mAnimationPool.remove(entity);
    }

    Transform2DComponent& Scene2D::addTransformComponent(Entity2D entity) {
        return mTransformPool.add(entity);
    }

    SpriteComponent& Scene2D::addSpriteComponent(Entity2D entity) {
        return mSpritePool.add(entity);
    }

    TextComponent& Scene2D::addTextComponent(Entity2D entity) {
        return mTextPool.add(entity);
    }

    Render2DComponent& Scene2D::addRenderComponent(Entity2D entity) {
        Render2DComponent& component = mRenderPool.add(entity);
        component.setLayer(mDefaultLayer.get());
        return component;
    }

    Animation2DComponent& Scene2D::addAnimationComponent(Entity2D entity) {
        return mAnimationPool.add(entity);
    }

    Transform2DComponent* Scene2D::getTransformComponent(Entity2D entity) {
        return mTransformPool.get(entity);
    }

    SpriteComponent* Scene2D::getSpriteComponent(Entity2D entity) {
        return mSpritePool.get(entity);
    }

    TextComponent* Scene2D::getTextComponent(Entity2D entity) {
        return mTextPool.get(entity);
    }

    Render2DComponent* Scene2D::getRenderComponent(Entity2D entity) {
        return mRenderPool.get(entity);
    }

    Animation2DComponent* Scene2D::getAnimationComponent(Entity2D entity) {
        return mAnimationPool.get(entity);
    }

    Layer2D* Scene2D::createLayer(const std::string& name) {
        if (mLayers.find(name) != mLayers.end()) {
            return nullptr; // Layer already exists
        }
        
        auto layer = std::make_unique<Layer2D>();
        layer->setName(name);
        Layer2D* result = layer.get();
        mLayers[name] = std::move(layer);
        return result;
    }

    bool Scene2D::destroyLayer(const std::string& name) {
        auto it = mLayers.find(name);
        if (it != mLayers.end()) {
            it->second->shutdown();
            mLayers.erase(it);
            return true;
        }
        return false;
    }

    Layer2D* Scene2D::getLayer(const std::string& name) const {
        auto it = mLayers.find(name);
        if (it != mLayers.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    Layer2D* Scene2D::getDefaultLayer() const {
        return mDefaultLayer.get();
    }

    Camera2D& Scene2D::getMainCamera() {
        return mMainCamera;
    }

    const Camera2D& Scene2D::getMainCamera() const {
        return mMainCamera;
    }

    void Scene2D::setViewportSize(uint32_t width, uint32_t height) {
        mViewportWidth = width;
        mViewportHeight = height;
        mMainCamera.resize(0, 0, width, height);
    }

    uint32_t Scene2D::getViewportWidth() const {
        return mViewportWidth;
    }

    uint32_t Scene2D::getViewportHeight() const {
        return mViewportHeight;
    }

    bool Scene2D::isValid(Entity2D entity) const {
        return entity.id != 0;
    }

    const std::string& Scene2D::getName() const {
        return mName;
    }

    void Scene2D::setName(const std::string& name) {
        mName = name;
    }

    Scene2DManager* Scene2D::getManager() const {
        return mManager;
    }

    //---------------------------------------------------------------------------------------------
    // SimpleScene2D implementation
    //---------------------------------------------------------------------------------------------

    SimpleScene2D::SimpleScene2D() {
        // empty
    }

    bool SimpleScene2D::init(renderer::RHI& rhi) {
        if (!Scene2D::init(rhi)) {
            return false;
        }
        return true;
    }

    Entity2D SimpleScene2D::createSprite(core::guid textureHandle, const glm::vec2& position, const glm::vec2& size) {
        Entity2D entity = createEntity();
        
        Transform2DComponent& transform = addTransformComponent(entity);
        transform.setPosition(position);
        transform.setSize(size);
        
        SpriteComponent& sprite = addSpriteComponent(entity);
        sprite.setTexture(textureHandle);
        
        Render2DComponent& render = addRenderComponent(entity);
        render.setVisible(true);
        
        return entity;
    }

} // namespace segfault::scene
