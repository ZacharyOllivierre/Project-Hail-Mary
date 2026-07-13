#pragma once

#include "../core/geometry/rect.h"
#include "../core/scene_object.h"
#include "tile_collision_world.h"
#include "../core/interface/collidable.h"
#include "../core/interface/kinematic_body.h"

#include <vector>

namespace engine::physics
{

class PhysicsManager
{
public:
    void set_collision_world(const TileCollisionWorld* world) noexcept;
    void clear_collision_world() noexcept;

    void register_body(
        ::engine::core::SceneObject* owner,
        ::engine::core::KinematicBody* body,
        ::engine::core::Collidable* collider
    ) noexcept;
    void unregister_body(const ::engine::core::SceneObject* owner) noexcept;
    void clear_bodies() noexcept;

    void step(double delta) noexcept;

private:
    struct BodyEntry
    {
        ::engine::core::SceneObject* owner = nullptr;
        ::engine::core::KinematicBody* body = nullptr;
        ::engine::core::Collidable* collider = nullptr;
    };

    void remove_invalid_entries() noexcept;

private:
    const TileCollisionWorld* _collision_world = nullptr;
    std::vector<BodyEntry> _bodies;
};
}
