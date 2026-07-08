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
#ifndef SEGFAULT_CAMERA_H
#define SEGFAULT_CAMERA_H

#include "core/segfault.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace segfault::scene {

    class Camera {
    public:
        enum class ProjectionMode {
            Invalid = -1,
            Perspective,
            Orthographic,
            Count
        };

        Camera();
        void lookAt(glm::vec3 position);
        void yaw(float angle);
        void pitch(float angle);
        void roll(float angle);

        glm::vec3 mPosition{0.0f, 0.0f, 3.0f};
        glm::vec3 mLookAt{0.0f, 0.0f, 0.0f};
        glm::vec3 mUp{0.0f, 1.0f, 0.0f};
        float mFov;
        ProjectionMode mProjectionMode{ProjectionMode::Perspective};
        glm::mat4 mViewMatrix{};
    };

    inline Camera::Camera() {
        mViewMatrix = 1.0f;
    }

    inline void Camera::lookAt() {
        mViewMatrix = glm::lookAt(mPosition, mLookAt, mUp);
    }

    inline void Camera::yaw(float angle) {
        const glm::vec3 targetPos = mLookAt - mPosition;
        mViewMatrix = glm::rotate(mViewMatrix, glm::radians(angle), targetPos);

    }
    inline void Camera::pitch(float angle) {
        glm::vec3 right = glm::normalize(glm::cross(mUp, mLookAt - mPosition));
        mViewMatrix = glm::rotate(mViewMatrix, glm::radians(angle), right);
    }

    inline void Camera::roll(float angle) {
        mViewMatrix = glm::rotate(mViewMatrix, glm::radians(angle), mUp);
    }

}

#endif //SEGFAULT_CAMERA_H
