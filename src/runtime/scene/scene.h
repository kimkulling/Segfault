#pragma once

#include "core/segfault.h"
#include "core/segfaultexception.h"
#include "cppcore/Container/TArray.h"
#include "renderer/RHI.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <unordered_map>

namespace segfault::scene {

    struct Entity {
        core::guid id{0};
        bool isValid() const {
            return id != 0;
        }

        bool operator ==(const Entity& other) const {
            return id == other.id;
        }
    };

    template<class T>
    class ComponentPool {
    public:
        T &add(Entity e) {
            const uint32_t idx = mDense.size();
            mEntityToIndex[e.id] = idx;
            mIndexToEntity.add(e);
            mDense.add(T{});
            return mDense[idx];
        }

        T *get(Entity e) {
            auto it = mEntityToIndex.find(e.id);
            return it == mEntityToIndex.end() ? nullptr : &mDense[it->second];
        }

        void remove(Entity e);          // swap-and-pop to keep mDense contiguous
        cppcore::TArray<T> &data() { return mDense; }   // systems iterate this

    private:
        cppcore::TArray<T> mDense;                    // packed components
        cppcore::TArray<Entity> mIndexToEntity;       // parallel: dense idx -> entity
        std::unordered_map<core::guid, uint32_t> mEntityToIndex;
    };

    struct TransformComponent {
        glm::vec3 mPosition{0.0f};
        glm::quat mRotation{1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 mScale{1.0f};
        glm::mat4 mWorld{1.0f};   // cached, recomputed by TransformSystem
        Entity    mParent{};      // for hierarchy
    };

    struct MeshRendererComponent {
        guid mMeshHandle{0};
        guid mMaterialHandle{0};
        bool mVisible{true};
    };

    class Scene {
    public:
        ~Scene() = default;
        void destroyEntity(Entity e);

        template <typename T> T &addComponent(Entity e);
        template <typename T> T *getComponent(Entity e);
        template <typename T> ComponentPool<T> &pool();

        void update(float deltaTime);   // runs systems in order
        void render(renderer::RHI &rhi);

    private:
        ComponentPool<TransformComponent> mTransformPool;
        ComponentPool<MeshRendererComponent> mMeshRendererPool;
    };

} // namespace segfault::scene  