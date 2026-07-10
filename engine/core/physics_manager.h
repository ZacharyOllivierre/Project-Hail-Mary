#pragma once

#include "geometry/rect.h"
#include "scene_object.h"
#include "tile_collision_world.h"
#include "interface/collidable.h"
#include "interface/kinematic_body.h"

#include <vector>

class PhysicsManager
{
public:
    enum class DebugRectType
    {
        Collider,
        SubstepCollider,
        HorizontalCandidate,
        VerticalCandidate,
        BlockingTile
    };

    struct DebugRect
    {
        Rect rect{};
        DebugRectType type = DebugRectType::Collider;
    };

    void set_collision_world(const TileCollisionWorld* world) noexcept;
    void clear_collision_world() noexcept;

    void set_debug_enabled(bool enabled) noexcept;
    [[nodiscard]] bool debug_enabled() const noexcept;
    [[nodiscard]] const std::vector<DebugRect>& debug_snapshot() const noexcept;

    void register_body(
        SceneObject* owner,
        KinematicBody* body,
        Collidable* collider
    ) noexcept;
    void unregister_body(const SceneObject* owner) noexcept;
    void clear_bodies() noexcept;

    void step(double delta) noexcept;

private:
    struct BodyEntry
    {
        SceneObject* owner = nullptr;
        KinematicBody* body = nullptr;
        Collidable* collider = nullptr;
    };

    void remove_invalid_entries() noexcept;

private:
    const TileCollisionWorld* _collision_world = nullptr;
    std::vector<BodyEntry> _bodies;
    bool _debug_enabled = false;
    std::vector<DebugRect> _debug_snapshot;
};
