#pragma once

#include "wand_types.h"

#include "../../engine/core/game_object.h"
#include "../../engine/physics/collision_box.h"

#include <expected>
#include <vector>


struct ProjectileCollisionProfile
{
    engine::physics::CollisionLayer layer = engine::physics::CollisionLayer::None;
    engine::physics::CollisionTarget targets = engine::physics::CollisionTarget::None;

    [[nodiscard]] bool entity_collision_enabled() const noexcept
    {
        return targets != engine::physics::CollisionTarget::None;
    }

    [[nodiscard]] bool is_complete() const noexcept
    {
        const bool has_layer = layer != engine::physics::CollisionLayer::None;
        const bool has_targets = targets != engine::physics::CollisionTarget::None;
        return has_layer == has_targets;
    }
};

struct ProjectileFireRequest
{
    const engine::core::GameObject* source = nullptr;
    ProjectileCollisionProfile collision;
    std::vector<ShotDescriptor> shots;
};
