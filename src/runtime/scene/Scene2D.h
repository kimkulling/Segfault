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
#include "core/ifilemanager.h"
#include "renderer/RHI.h"

#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <functional>
#include <memory>

namespace segfault::scene {

    // Forward declarations
    class Scene2DManager;
    class Layer2D;

    //---------------------------------------------------------------------------------------------
    /// @brief 2D Camera specifically designed for orthographic projection.
    ///
    /// Supports zooming, panning, and viewport operations for 2D rendering.
    //---------------------------------------------------------------------------------------------
    class SEGFAULT_EXPORT Camera2D final {
    public:
        /// @brief Constructs a new Camera2D with default orthographic projection.
        Camera2D();

        /// @brief Destroys the Camera2D.
        ~Camera2D() = default;

        //-----------------------------------------------------------------------------------------
        /// @brief Sets up an orthographic projection.
        /// @param[ in ] left The left coordinate of the viewport.
        /// @param[ in ] right The right coordinate of the viewport.
        /// @param[ in ] bottom The bottom coordinate of the viewport.
        /// @param[ in ] top The top coordinate of the viewport.
        /// @param[ in ] near The near clipping plane.
        /// @param[ in ] far The far clipping plane.
        //-----------------------------------------------------------------------------------------
        void setOrthographic(float left, float right, float bottom, float top, float near = -1.0f, float far = 1.0f);

        //-----------------------------------------------------------------------------------------
        /// @brief Sets up an orthographic projection based on viewport size.
        /// @param[ in ] width The width of the viewport.
        /// @param[ in ] height The height of the viewport.
        /// @param[ in ] near The near clipping plane.
        /// @param[ in ] far The far clipping plane.
        //-----------------------------------------------------------------------------------------
        void setOrthographicFromViewport(uint32_t width, uint32_t height, float near = -1.0f, float far = 1.0f);

        //-----------------------------------------------------------------------------------------
        /// @brief Sets the position of the camera.
        /// @param[ in ] position The 2D position of the camera.
        //-----------------------------------------------------------------------------------------
        void setPosition(const glm::vec2& position);

        //-----------------------------------------------------------------------------------------
        /// @brief Sets the position of the camera.
        /// @param[ in ] x The x-coordinate of the camera position.
        /// @param[ in ] y The y-coordinate of the camera position.
        //-----------------------------------------------------------------------------------------
        void setPosition(float x, float y);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the position of the camera.
        /// @return The 2D position of the camera.
        //-----------------------------------------------------------------------------------------
        const glm::vec2& getPosition() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Sets the zoom level of the camera.
        /// @param[ in ] zoom The zoom level (1.0 = normal, >1.0 = zoomed in, <1.0 = zoomed out).
        //-----------------------------------------------------------------------------------------
        void setZoom(float zoom);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the zoom level of the camera.
        /// @return The current zoom level.
        //-----------------------------------------------------------------------------------------
        float getZoom() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Sets the rotation of the camera in degrees.
        /// @param[ in ] angle The rotation angle in degrees.
        //-----------------------------------------------------------------------------------------
        void setRotation(float angle);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the rotation of the camera in degrees.
        /// @return The current rotation angle in degrees.
        //-----------------------------------------------------------------------------------------
        float getRotation() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the view matrix.
        /// @return The view matrix.
        //-----------------------------------------------------------------------------------------
        const glm::mat4& getViewMatrix() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the projection matrix.
        /// @return The projection matrix.
        //-----------------------------------------------------------------------------------------
        const glm::mat4& getProjectionMatrix() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the combined view-projection matrix.
        /// @return The view-projection matrix.
        //-----------------------------------------------------------------------------------------
        glm::mat4 getViewProjectionMatrix() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Converts screen coordinates to world coordinates.
        /// @param[ in ] screenPos The screen position (in pixels).
        /// @param[ in ] viewportWidth The width of the viewport in pixels.
        /// @param[ in ] viewportHeight The height of the viewport in pixels.
        /// @return The corresponding world position.
        //-----------------------------------------------------------------------------------------
        glm::vec2 screenToWorld(const glm::vec2& screenPos, uint32_t viewportWidth, uint32_t viewportHeight) const;

        //-----------------------------------------------------------------------------------------
        /// @brief Converts world coordinates to screen coordinates.
        /// @param[ in ] worldPos The world position.
        /// @param[ in ] viewportWidth The width of the viewport in pixels.
        /// @param[ in ] viewportHeight The height of the viewport in pixels.
        /// @return The corresponding screen position (in pixels).
        //-----------------------------------------------------------------------------------------
        glm::vec2 worldToScreen(const glm::vec2& worldPos, uint32_t viewportWidth, uint32_t viewportHeight) const;

        //-----------------------------------------------------------------------------------------
        /// @brief Moves the camera by the specified offset.
        /// @param[ in ] offset The offset to move the camera by.
        //-----------------------------------------------------------------------------------------
        void move(const glm::vec2& offset);

        //-----------------------------------------------------------------------------------------
        /// @brief Resizes the camera viewport.
        /// @param[ in ] width The new width of the viewport.
        /// @param[ in ] height The new height of the viewport.
        //-----------------------------------------------------------------------------------------
        void resize(uint32_t width, uint32_t height);

        //-----------------------------------------------------------------------------------------
        /// @brief Sets the viewport boundaries.
        /// @param[ in ] x The x-coordinate of the viewport.
        /// @param[ in ] y The y-coordinate of the viewport.
        /// @param[ in ] width The width of the viewport.
        /// @param[ in ] height The height of the viewport.
        //-----------------------------------------------------------------------------------------
        void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the viewport rectangle.
        /// @return The viewport rectangle.
        //-----------------------------------------------------------------------------------------
        const core::Rect& getViewport() const;

    private:
        glm::vec2 mPosition{ 0.0f, 0.0f };
        float mZoom{ 1.0f };
        float mRotation{ 0.0f };
        glm::mat4 mViewMatrix{ 1.0f };
        glm::mat4 mProjectionMatrix{ 1.0f };
        core::Rect mViewport{ 0, 0, 0, 0 };

        bool mDirty{ true };

        void updateMatrices();
    };

    //---------------------------------------------------------------------------------------------
    /// @brief Base class for all 2D layers.
    ///
    /// Layers are used to organize 2D rendering into separate groups with their own
    /// rendering order, camera, and visibility settings.
    //---------------------------------------------------------------------------------------------
    class SEGFAULT_EXPORT Layer2D {
    public:
        /// @brief Constructs a new Layer2D.
        Layer2D();

        /// @brief Destroys the Layer2D.
        virtual ~Layer2D() = default;

        /// @brief Initializes the layer.
        /// @param[ in ] rhi The rendering hardware interface.
        /// @return True if initialization was successful, false otherwise.
        virtual bool init(renderer::RHI& rhi);

        /// @brief Shuts down the layer and releases resources.
        virtual void shutdown();

        /// @brief Updates the layer.
        /// @param[ in ] deltaTime The time elapsed since the last frame.
        virtual void update(float deltaTime);

        /// @brief Renders the layer.
        /// @param[ in ] rhi The rendering hardware interface.
        virtual void render(renderer::RHI& rhi);

        //-----------------------------------------------------------------------------------------
        /// @brief Sets the name of the layer.
        /// @param[ in ] name The name of the layer.
        //-----------------------------------------------------------------------------------------
        void setName(const std::string& name);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the name of the layer.
        /// @return The name of the layer.
        //-----------------------------------------------------------------------------------------
        const std::string& getName() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Sets the visibility of the layer.
        /// @param[ in ] visible True to make the layer visible, false to hide it.
        //-----------------------------------------------------------------------------------------
        void setVisible(bool visible);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the visibility of the layer.
        /// @return True if the layer is visible, false otherwise.
        //-----------------------------------------------------------------------------------------
        bool isVisible() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Sets the rendering order of the layer (lower values render first).
        /// @param[ in ] order The rendering order.
        //-----------------------------------------------------------------------------------------
        void setRenderOrder(int32_t order);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the rendering order of the layer.
        /// @return The rendering order.
        //-----------------------------------------------------------------------------------------
        int32_t getRenderOrder() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Sets the camera for this layer.
        /// @param[ in ] camera The camera to use for rendering this layer.
        //-----------------------------------------------------------------------------------------
        void setCamera(Camera2D* camera);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the camera for this layer.
        /// @return Pointer to the camera, or nullptr if using the default camera.
        //-----------------------------------------------------------------------------------------
        Camera2D* getCamera() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Sets whether this layer uses its own camera or the scene's default camera.
        /// @param[ in ] useOwnCamera True to use the layer's own camera, false to use the scene's camera.
        //-----------------------------------------------------------------------------------------
        void setUseOwnCamera(bool useOwnCamera);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets whether this layer uses its own camera.
        /// @return True if using own camera, false otherwise.
        //-----------------------------------------------------------------------------------------
        bool getUseOwnCamera() const;

    protected:
        std::string mName;
        bool mVisible{ true };
        int32_t mRenderOrder{ 0 };
        Camera2D* mCamera{ nullptr };
        bool mUseOwnCamera{ false };
    };

    //---------------------------------------------------------------------------------------------
    /// @brief Entity handle for 2D scene objects.
    //---------------------------------------------------------------------------------------------
    struct SEGFAULT_EXPORT Entity2D {
        core::guid id{ 0 };

        /// @brief Checks if the entity is valid.
        /// @return True if the entity is valid, false otherwise.
        bool isValid() const;

        /// @brief Comparison operator.
        /// @param[ in ] other The other entity to compare with.
        /// @return True if the entities are equal, false otherwise.
        bool operator==(const Entity2D& other) const;

        /// @brief Comparison operator.
        /// @param[ in ] other The other entity to compare with.
        /// @return True if the entities are not equal, false otherwise.
        bool operator!=(const Entity2D& other) const;
    };

    //---------------------------------------------------------------------------------------------
    /// @brief Transform component for 2D entities.
    ///
    /// Stores position, rotation, scale, and other transform properties for 2D rendering.
    //---------------------------------------------------------------------------------------------
    struct SEGFAULT_EXPORT Transform2DComponent final {
        glm::vec2 mPosition{ 0.0f, 0.0f };
        float mRotation{ 0.0f }; // in degrees
        glm::vec2 mScale{ 1.0f, 1.0f };
        glm::vec2 mOrigin{ 0.0f, 0.0f }; // pivot point (0-1 normalized)
        glm::vec2 mSize{ 1.0f, 1.0f };
        glm::mat4 mWorldMatrix{ 1.0f };
        bool mDirty{ true };

        /// @brief Updates the world matrix based on current transform properties.
        void updateWorldMatrix();

        /// @brief Sets the position.
        /// @param[ in ] position The new position.
        void setPosition(const glm::vec2& position);

        /// @brief Sets the rotation.
        /// @param[ in ] rotation The new rotation in degrees.
        void setRotation(float rotation);

        /// @brief Sets the scale.
        /// @param[ in ] scale The new scale.
        void setScale(const glm::vec2& scale);

        /// @brief Sets the size.
        /// @param[ in ] size The new size.
        void setSize(const glm::vec2& size);

        /// @brief Sets the origin/pivot point (normalized 0-1).
        /// @param[ in ] origin The new origin.
        void setOrigin(const glm::vec2& origin);
    };

    //---------------------------------------------------------------------------------------------
    /// @brief Sprite component for 2D rendering.
    ///
    /// Represents a textured quad that can be rendered with a texture.
    //---------------------------------------------------------------------------------------------
    struct SEGFAULT_EXPORT SpriteComponent final {
        core::guid mTextureHandle{ 0 };
        core::Rect mTextureRect{ 0, 0, 0, 0 };
        glm::vec4 mColor{ 1.0f, 1.0f, 1.0f, 1.0f };
        bool mFlipX{ false };
        bool mFlipY{ false };
        float mAlpha{ 1.0f };
        bool mVisible{ true };

        /// @brief Default constructor.
        SpriteComponent() = default;

        /// @brief Constructs a sprite component with a texture.
        /// @param[ in ] textureHandle The handle to the texture.
        SpriteComponent(core::guid textureHandle);

        /// @brief Sets the texture handle.
        /// @param[ in ] textureHandle The handle to the texture.
        void setTexture(core::guid textureHandle);

        /// @brief Sets the color.
        /// @param[ in ] color The color to apply to the sprite.
        void setColor(const glm::vec4& color);

        /// @brief Sets the alpha value.
        /// @param[ in ] alpha The alpha value (0.0 - 1.0).
        void setAlpha(float alpha);

        /// @brief Sets the texture rectangle (for sprite sheets).
        /// @param[ in ] rect The texture rectangle.
        void setTextureRect(const core::Rect& rect);

        /// @brief Sets the flip flags.
        /// @param[ in ] flipX True to flip horizontally.
        /// @param[ in ] flipY True to flip vertically.
        void setFlip(bool flipX, bool flipY);

        /// @brief Sets visibility.
        /// @param[ in ] visible True to make visible, false to hide.
        void setVisible(bool visible);
    };

    //---------------------------------------------------------------------------------------------
    /// @brief Text component for 2D rendering.
    ///
    /// Represents text that can be rendered using a font.
    //---------------------------------------------------------------------------------------------
    struct SEGFAULT_EXPORT TextComponent final {
        std::string mText;
        core::guid mFontHandle{ 0 };
        glm::vec4 mColor{ 1.0f, 1.0f, 1.0f, 1.0f };
        float mFontSize{ 16.0f };
        float mLineSpacing{ 1.0f };
        float mLetterSpacing{ 0.0f };
        bool mVisible{ true };
        bool mDirty{ true };

        // Text alignment
        enum class TextAlignment {
            Left,
            Center,
            Right
        };

        enum class VerticalAlignment {
            Top,
            Middle,
            Bottom
        };

        TextAlignment mHorizontalAlignment{ TextAlignment::Left };
        VerticalAlignment mVerticalAlignment{ VerticalAlignment::Top };

        /// @brief Default constructor.
        TextComponent() = default;

        /// @brief Constructs a text component with text and font.
        /// @param[ in ] text The text to display.
        /// @param[ in ] fontHandle The handle to the font.
        TextComponent(const std::string& text, core::guid fontHandle);

        /// @brief Sets the text.
        /// @param[ in ] text The text to display.
        void setText(const std::string& text);

        /// @brief Sets the font handle.
        /// @param[ in ] fontHandle The handle to the font.
        void setFont(core::guid fontHandle);

        /// @brief Sets the color.
        /// @param[ in ] color The color to apply to the text.
        void setColor(const glm::vec4& color);

        /// @brief Sets the font size.
        /// @param[ in ] fontSize The font size in points.
        void setFontSize(float fontSize);

        /// @brief Sets the horizontal alignment.
        /// @param[ in ] alignment The horizontal alignment.
        void setHorizontalAlignment(TextAlignment alignment);

        /// @brief Sets the vertical alignment.
        /// @param[ in ] alignment The vertical alignment.
        void setVerticalAlignment(VerticalAlignment alignment);

        /// @brief Sets visibility.
        /// @param[ in ] visible True to make visible, false to hide.
        void setVisible(bool visible);
    };

    //---------------------------------------------------------------------------------------------
    /// @brief UI Sprite component for 2D user interface elements.
    ///
    /// Extended sprite component with UI-specific features like nine-slice scaling.
    //---------------------------------------------------------------------------------------------
    struct SEGFAULT_EXPORT UISpriteComponent final {
        core::guid mTextureHandle{ 0 };
        core::Rect mTextureRect{ 0, 0, 0, 0 };
        glm::vec4 mColor{ 1.0f, 1.0f, 1.0f, 1.0f };
        float mAlpha{ 1.0f };
        bool mVisible{ true };

        // Nine-slice border settings
        uint32_t mBorderLeft{ 0 };
        uint32_t mBorderRight{ 0 };
        uint32_t mBorderTop{ 0 };
        uint32_t mBorderBottom{ 0 };

        // UI-specific settings
        bool mInteractive{ false };
        bool mEnabled{ true };
        bool mSelected{ false };
        bool mPressed{ false };
        bool mHovered{ false };

        // Callback functions for UI events
        std::function<void()> mOnClick;
        std::function<void()> mOnHoverEnter;
        std::function<void()> mOnHoverExit;

        /// @brief Default constructor.
        UISpriteComponent() = default;

        /// @brief Sets the texture handle.
        /// @param[ in ] textureHandle The handle to the texture.
        void setTexture(core::guid textureHandle);

        /// @brief Sets the nine-slice borders.
        /// @param[ in ] left The left border width.
        /// @param[ in ] right The right border width.
        /// @param[ in ] top The top border height.
        /// @param[ in ] bottom The bottom border height.
        void setNineSliceBorders(uint32_t left, uint32_t right, uint32_t top, uint32_t bottom);

        /// @brief Sets visibility.
        /// @param[ in ] visible True to make visible, false to hide.
        void setVisible(bool visible);

        /// @brief Sets interactiveness.
        /// @param[ in ] interactive True to enable interaction, false to disable.
        void setInteractive(bool interactive);

        /// @brief Sets enabled state.
        /// @param[ in ] enabled True to enable, false to disable.
        void setEnabled(bool enabled);

        /// @brief Sets the click callback.
        /// @param[ in ] callback The function to call when clicked.
        void setOnClick(std::function<void()> callback);

        /// @brief Sets the hover enter callback.
        /// @param[ in ] callback The function to call when hovered over.
        void setOnHoverEnter(std::function<void()> callback);

        /// @brief Sets the hover exit callback.
        /// @param[ in ] callback The function to call when hover exits.
        void setOnHoverExit(std::function<void()> callback);

        /// @brief Simulates a click event.
        void onClick();

        /// @brief Simulates a hover enter event.
        void onHoverEnter();

        /// @brief Simulates a hover exit event.
        void onHoverExit();
    };

    //---------------------------------------------------------------------------------------------
    /// @brief Render component for 2D entities.
    ///
    /// Contains render-specific data like layer assignment and render order.
    //---------------------------------------------------------------------------------------------
    struct SEGFAULT_EXPORT Render2DComponent final {
        Layer2D* mLayer{ nullptr };
        int32_t mRenderOrder{ 0 };
        bool mVisible{ true };
        bool mCastShadows{ false };
        bool mReceiveShadows{ false };

        /// @brief Default constructor.
        Render2DComponent() = default;

        /// @brief Sets the layer.
        /// @param[ in ] layer The layer this entity belongs to.
        void setLayer(Layer2D* layer);

        /// @brief Sets the render order within the layer.
        /// @param[ in ] order The render order (lower values render first).
        void setRenderOrder(int32_t order);

        /// @brief Sets visibility.
        /// @param[ in ] visible True to make visible, false to hide.
        void setVisible(bool visible);
    };

    //---------------------------------------------------------------------------------------------
    /// @brief Animation component for 2D entities.
    ///
    /// Supports frame-based animation for sprites.
    //---------------------------------------------------------------------------------------------
    struct SEGFAULT_EXPORT Animation2DComponent final {
        struct Frame final {
            core::Rect mTextureRect;
            float mDuration{ 0.1f }; // in seconds
        };

        std::vector<Frame> mFrames;
        uint32_t mCurrentFrame{ 0 };
        float mFrameTimer{ 0.0f };
        float mAnimationSpeed{ 1.0f };
        bool mPlaying{ false };
        bool mLoop{ true };
        bool mDirty{ true };

        /// @brief Default constructor.
        Animation2DComponent() = default;

        /// @brief Adds a frame to the animation.
        /// @param[ in ] textureRect The texture rectangle for this frame.
        /// @param[ in ] duration The duration of this frame in seconds.
        void addFrame(const core::Rect& textureRect, float duration = 0.1f);

        /// @brief Sets the animation frames.
        /// @param[ in ] frames The array of frames.
        void setFrames(const std::vector<Frame>& frames);

        /// @brief Plays the animation.
        /// @param[ in ] loop True to loop the animation, false to play once.
        void play(bool loop = true);

        /// @brief Stops the animation.
        void stop();

        /// @brief Pauses the animation.
        void pause();

        /// @brief Resumes the animation.
        void resume();

        /// @brief Updates the animation.
        /// @param[ in ] deltaTime The time elapsed since the last frame.
        void update(float deltaTime);

        /// @brief Gets the current frame's texture rectangle.
        /// @return The texture rectangle of the current frame.
        core::Rect getCurrentFrameRect() const;

        /// @brief Gets the current frame index.
        /// @return The current frame index.
        uint32_t getCurrentFrameIndex() const;

        /// @brief Sets the current frame index.
        /// @param[ in ] frameIndex The frame index to set.
        void setCurrentFrameIndex(uint32_t frameIndex);

        /// @brief Sets the animation speed.
        /// @param[ in ] speed The animation speed multiplier.
        void setAnimationSpeed(float speed);

        /// @brief Checks if the animation is playing.
        /// @return True if playing, false otherwise.
        bool isPlaying() const;

        /// @brief Checks if the animation is looped.
        /// @return True if looped, false otherwise.
        bool isLooping() const;

        /// @brief Gets the number of frames.
        /// @return The number of frames.
        uint32_t getFrameCount() const;
    };

    //---------------------------------------------------------------------------------------------
    /// @brief Base class for all 2D scenes.
    ///
    /// Provides the foundation for creating custom 2D scenes with entities, layers, and rendering.
    //---------------------------------------------------------------------------------------------
    class SEGFAULT_EXPORT Scene2D {
    public:
        /// @brief Constructs a new Scene2D.
        Scene2D();

        /// @brief Destroys the Scene2D.
        virtual ~Scene2D();

        /// @brief Initializes the scene.
        /// @param[ in ] rhi The rendering hardware interface.
        /// @return True if initialization was successful, false otherwise.
        virtual bool init(renderer::RHI& rhi);

        /// @brief Shuts down the scene and releases all resources.
        virtual void shutdown();

        /// @brief Updates the scene.
        /// @param[ in ] deltaTime The time elapsed since the last frame.
        virtual void update(float deltaTime);

        /// @brief Renders the scene.
        /// @param[ in ] rhi The rendering hardware interface.
        virtual void render(renderer::RHI& rhi);

        /// @brief Called when the scene is activated.
        virtual void onActivate();

        /// @brief Called when the scene is deactivated.
        virtual void onDeactivate();

        /// @brief Called when the scene is loaded.
        virtual void onLoad();

        /// @brief Called when the scene is unloaded.
        virtual void onUnload();

        //-----------------------------------------------------------------------------------------
        /// @brief Creates a new 2D entity.
        /// @return A new entity handle.
        //-----------------------------------------------------------------------------------------
        Entity2D createEntity();

        //-----------------------------------------------------------------------------------------
        /// @brief Destroys an entity and all its components.
        /// @param[ in ] entity The entity to destroy.
        //-----------------------------------------------------------------------------------------
        void destroyEntity(Entity2D entity);

        //-----------------------------------------------------------------------------------------
        /// @brief Adds a transform component to an entity.
        /// @param[ in ] entity The entity to add the component to.
        /// @return Reference to the added component.
        //-----------------------------------------------------------------------------------------
        Transform2DComponent& addTransformComponent(Entity2D entity);

        //-----------------------------------------------------------------------------------------
        /// @brief Adds a sprite component to an entity.
        /// @param[ in ] entity The entity to add the component to.
        /// @return Reference to the added component.
        //-----------------------------------------------------------------------------------------
        SpriteComponent& addSpriteComponent(Entity2D entity);

        //-----------------------------------------------------------------------------------------
        /// @brief Adds a text component to an entity.
        /// @param[ in ] entity The entity to add the component to.
        /// @return Reference to the added component.
        //-----------------------------------------------------------------------------------------
        TextComponent& addTextComponent(Entity2D entity);

        //-----------------------------------------------------------------------------------------
        /// @brief Adds a UI sprite component to an entity.
        /// @param[ in ] entity The entity to add the component to.
        /// @return Reference to the added component.
        //-----------------------------------------------------------------------------------------
        UISpriteComponent& addUISpriteComponent(Entity2D entity);

        //-----------------------------------------------------------------------------------------
        /// @brief Adds a render component to an entity.
        /// @param[ in ] entity The entity to add the component to.
        /// @return Reference to the added component.
        //-----------------------------------------------------------------------------------------
        Render2DComponent& addRenderComponent(Entity2D entity);

        //-----------------------------------------------------------------------------------------
        /// @brief Adds an animation component to an entity.
        /// @param[ in ] entity The entity to add the component to.
        /// @return Reference to the added component.
        //-----------------------------------------------------------------------------------------
        Animation2DComponent& addAnimationComponent(Entity2D entity);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets a transform component from an entity.
        /// @param[ in ] entity The entity to get the component from.
        /// @return Pointer to the component, or nullptr if not found.
        //-----------------------------------------------------------------------------------------
        Transform2DComponent* getTransformComponent(Entity2D entity);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets a sprite component from an entity.
        /// @param[ in ] entity The entity to get the component from.
        /// @return Pointer to the component, or nullptr if not found.
        //-----------------------------------------------------------------------------------------
        SpriteComponent* getSpriteComponent(Entity2D entity);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets a text component from an entity.
        /// @param[ in ] entity The entity to get the component from.
        /// @return Pointer to the component, or nullptr if not found.
        //-----------------------------------------------------------------------------------------
        TextComponent* getTextComponent(Entity2D entity);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets a UI sprite component from an entity.
        /// @param[ in ] entity The entity to get the component from.
        /// @return Pointer to the component, or nullptr if not found.
        //-----------------------------------------------------------------------------------------
        UISpriteComponent* getUISpriteComponent(Entity2D entity);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets a render component from an entity.
        /// @param[ in ] entity The entity to get the component from.
        /// @return Pointer to the component, or nullptr if not found.
        //-----------------------------------------------------------------------------------------
        Render2DComponent* getRenderComponent(Entity2D entity);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets an animation component from an entity.
        /// @param[ in ] entity The entity to get the component from.
        /// @return Pointer to the component, or nullptr if not found.
        //-----------------------------------------------------------------------------------------
        Animation2DComponent* getAnimationComponent(Entity2D entity);

        //-----------------------------------------------------------------------------------------
        /// @brief Creates a new layer.
        /// @param[ in ] name The name of the layer.
        /// @return Pointer to the created layer, or nullptr if creation failed.
        //-----------------------------------------------------------------------------------------
        Layer2D* createLayer(const std::string& name);

        //-----------------------------------------------------------------------------------------
        /// @brief Destroys a layer by name.
        /// @param[ in ] name The name of the layer to destroy.
        /// @return True if the layer was found and destroyed, false otherwise.
        //-----------------------------------------------------------------------------------------
        bool destroyLayer(const std::string& name);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets a layer by name.
        /// @param[ in ] name The name of the layer to retrieve.
        /// @return Pointer to the layer, or nullptr if not found.
        //-----------------------------------------------------------------------------------------
        Layer2D* getLayer(const std::string& name) const;

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the default layer.
        /// @return Pointer to the default layer.
        //-----------------------------------------------------------------------------------------
        Layer2D* getDefaultLayer() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the main camera.
        /// @return Reference to the main camera.
        //-----------------------------------------------------------------------------------------
        Camera2D& getMainCamera();

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the main camera (const version).
        /// @return Const reference to the main camera.
        //-----------------------------------------------------------------------------------------
        const Camera2D& getMainCamera() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Sets the viewport size.
        /// @param[ in ] width The width of the viewport.
        /// @param[ in ] height The height of the viewport.
        //-----------------------------------------------------------------------------------------
        void setViewportSize(uint32_t width, uint32_t height);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the viewport width.
        /// @return The viewport width.
        //-----------------------------------------------------------------------------------------
        uint32_t getViewportWidth() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the viewport height.
        /// @return The viewport height.
        //-----------------------------------------------------------------------------------------
        uint32_t getViewportHeight() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Checks if an entity is valid.
        /// @param[ in ] entity The entity to check.
        /// @return True if the entity is valid, false otherwise.
        //-----------------------------------------------------------------------------------------
        bool isValid(Entity2D entity) const;

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the scene name.
        /// @return The scene name.
        //-----------------------------------------------------------------------------------------
        const std::string& getName() const;

        //-----------------------------------------------------------------------------------------
        /// @brief Sets the scene name.
        /// @param[ in ] name The scene name.
        //-----------------------------------------------------------------------------------------
        void setName(const std::string& name);

        //-----------------------------------------------------------------------------------------
        /// @brief Gets the scene manager.
        /// @return Pointer to the scene manager, or nullptr if not assigned.
        //-----------------------------------------------------------------------------------------
        Scene2DManager* getManager() const;

    protected:
        // Component storage using dense arrays with entity mapping
        template<typename T>
        class ComponentPool2D {
        public:
            T& add(Entity2D entity) {
                const uint32_t idx = mDense.size();
                mEntityToIndex[entity.id] = idx;
                mIndexToEntity.push_back(entity);
                mDense.push_back(T{});
                return mDense[idx];
            }

            T* get(Entity2D entity) {
                auto it = mEntityToIndex.find(entity.id);
                return it == mEntityToIndex.end() ? nullptr : &mDense[it->second];
            }

            void remove(Entity2D entity) {
                auto it = mEntityToIndex.find(entity.id);
                if (it != mEntityToIndex.end()) {
                    uint32_t idx = it->second;
                    // Swap with last element for O(1) removal
                    if (idx < mDense.size() - 1) {
                        std::swap(mDense[idx], mDense.back());
                        mEntityToIndex[mIndexToEntity.back().id] = idx;
                        mIndexToEntity[idx] = mIndexToEntity.back();
                    }
                    mDense.pop_back();
                    mIndexToEntity.pop_back();
                    mEntityToIndex.erase(it);
                }
            }

            bool has(Entity2D entity) const {
                return mEntityToIndex.find(entity.id) != mEntityToIndex.end();
            }

            std::vector<T>& data() { return mDense; }
            const std::vector<T>& data() const { return mDense; }

        private:
            std::vector<T> mDense;
            std::vector<Entity2D> mIndexToEntity;
            std::unordered_map<core::guid, uint32_t> mEntityToIndex;
        };

        ComponentPool2D<Transform2DComponent> mTransformPool;
        ComponentPool2D<SpriteComponent> mSpritePool;
        ComponentPool2D<TextComponent> mTextPool;
        ComponentPool2D<UISpriteComponent> mUISpritePool;
        ComponentPool2D<Render2DComponent> mRenderPool;
        ComponentPool2D<Animation2DComponent> mAnimationPool;

        std::unordered_map<std::string, std::unique_ptr<Layer2D>> mLayers;
        std::unique_ptr<Layer2D> mDefaultLayer;
        Camera2D mMainCamera;
        std::string mName;
        Scene2DManager* mManager{ nullptr };
        uint32_t mViewportWidth{ 0 };
        uint32_t mViewportHeight{ 0 };
        core::guid mNextEntityId{ 1 };

        // Friend declaration for manager access
        friend class Scene2DManager;
    };

    //---------------------------------------------------------------------------------------------
    /// @brief Simple 2D scene for basic rendering.
    ///
    /// A convenience class for creating simple 2D scenes without complex setup.
    //---------------------------------------------------------------------------------------------
    class SEGFAULT_EXPORT SimpleScene2D : public Scene2D {
    public:
        /// @brief Constructs a new SimpleScene2D.
        SimpleScene2D();

        /// @brief Destroys the SimpleScene2D.
        ~SimpleScene2D() = default;

        /// @brief Initializes the simple scene.
        /// @param[ in ] rhi The rendering hardware interface.
        /// @return True if initialization was successful, false otherwise.
        bool init(renderer::RHI& rhi) override;

        /// @brief Creates a simple sprite entity.
        /// @param[ in ] textureHandle The handle to the texture.
        /// @param[ in ] position The position of the sprite.
        /// @param[ in ] size The size of the sprite.
        /// @return The created entity.
        Entity2D createSprite(core::guid textureHandle, const glm::vec2& position, const glm::vec2& size);

        /// @brief Creates a simple text entity.
        /// @param[ in ] text The text to display.
        /// @param[ in ] fontHandle The handle to the font.
        /// @param[ in ] position The position of the text.
        /// @param[ in ] fontSize The font size.
        /// @return The created entity.
        Entity2D createText(const std::string& text, core::guid fontHandle, const glm::vec2& position, float fontSize = 16.0f);

        /// @brief Creates a simple UI button entity.
        /// @param[ in ] textureHandle The handle to the button texture.
        /// @param[ in ] position The position of the button.
        /// @param[ in ] size The size of the button.
        /// @param[ in ] onClick The callback function when the button is clicked.
        /// @return The created entity.
        Entity2D createButton(core::guid textureHandle, const glm::vec2& position, const glm::vec2& size, std::function<void()> onClick);
    };

    //---------------------------------------------------------------------------------------------
    /// @brief UI Scene for user interface rendering.
    ///
    /// Specialized scene for UI elements with built-in input handling.
    //---------------------------------------------------------------------------------------------
    class SEGFAULT_EXPORT UIScene2D : public Scene2D {
    public:
        /// @brief Constructs a new UIScene2D.
        UIScene2D();

        /// @brief Destroys the UIScene2D.
        ~UIScene2D() = default;

        /// @brief Initializes the UI scene.
        /// @param[ in ] rhi The rendering hardware interface.
        /// @return True if initialization was successful, false otherwise.
        bool init(renderer::RHI& rhi) override;

        /// @brief Updates the UI scene with input.
        /// @param[ in ] deltaTime The time elapsed since the last frame.
        void update(float deltaTime) override;

        // Input handling
        virtual void onMouseMove(const glm::vec2& position);
        virtual void onMouseDown(const glm::vec2& position, uint8_t button);
        virtual void onMouseUp(const glm::vec2& position, uint8_t button);

        /// @brief Creates a UI panel.
        /// @param[ in ] textureHandle The handle to the panel texture.
        /// @param[ in ] position The position of the panel.
        /// @param[ in ] size The size of the panel.
        /// @return The created entity.
        Entity2D createPanel(core::guid textureHandle, const glm::vec2& position, const glm::vec2& size);

        /// @brief Creates a UI button.
        /// @param[ in ] textureHandle The handle to the button texture.
        /// @param[ in ] position The position of the button.
        /// @param[ in ] size The size of the button.
        /// @param[ in ] onClick The callback function when the button is clicked.
        /// @return The created entity.
        Entity2D createButton(core::guid textureHandle, const glm::vec2& position, const glm::vec2& size, std::function<void()> onClick);

        /// @brief Creates a UI text element.
        /// @param[ in ] text The text to display.
        /// @param[ in ] fontHandle The handle to the font.
        /// @param[ in ] position The position of the text.
        /// @param[ in ] fontSize The font size.
        /// @param[ in ] color The text color.
        /// @return The created entity.
        Entity2D createText(const std::string& text, core::guid fontHandle, const glm::vec2& position, float fontSize = 16.0f, const glm::vec4& color = glm::vec4(1.0f));

        /// @brief Creates a UI checkbox.
        /// @param[ in ] textureHandle The handle to the checkbox texture.
        /// @param[ in ] position The position of the checkbox.
        /// @param[ in ] size The size of the checkbox.
        /// @param[ in ] onChanged The callback function when the checkbox state changes.
        /// @return The created entity.
        Entity2D createCheckbox(core::guid textureHandle, const glm::vec2& position, const glm::vec2& size, std::function<void(bool)> onChanged);

        /// @brief Creates a UI slider.
        /// @param[ in ] backgroundTexture The handle to the background texture.
        /// @param[ in ] handleTexture The handle to the handle texture.
        /// @param[ in ] position The position of the slider.
        /// @param[ in ] size The size of the slider.
        /// @param[ in ] minValue The minimum value of the slider.
        /// @param[ in ] maxValue The maximum value of the slider.
        /// @param[ in ] onChanged The callback function when the slider value changes.
        /// @return The created entity.
        Entity2D createSlider(core::guid backgroundTexture, core::guid handleTexture, const glm::vec2& position, const glm::vec2& size, float minValue = 0.0f, float maxValue = 1.0f, std::function<void(float)> onChanged = nullptr);
    };

} // namespace segfault::scene
