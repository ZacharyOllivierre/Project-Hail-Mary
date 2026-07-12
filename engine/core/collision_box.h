#pragma once

#include "geometry/rect.h"

#include <cstdint>
#include <functional>
#include <utility>

class GameObject;

enum class CollisionLayer : std::uint32_t
{
    None = 0,
    World,
    Player,
    Enemy,
    PlayerProjectile,
    EnemyProjectile
};

enum class CollisionTarget : std::uint32_t
{
    None = 0,
    World = 1U << 0,
    Player = 1U << 1,
    Enemy = 1U << 2,
    PlayerProjectile = 1U << 3,
    EnemyProjectile = 1U << 4,
    All = (1U << 0) | (1U << 1) | (1U << 2) | (1U << 3) | (1U << 4)
};

[[nodiscard]] constexpr CollisionTarget to_collision_target(
    CollisionLayer layer) noexcept
{
    switch (layer)
    {
    case CollisionLayer::World:
        return CollisionTarget::World;
    case CollisionLayer::Player:
        return CollisionTarget::Player;
    case CollisionLayer::Enemy:
        return CollisionTarget::Enemy;
    case CollisionLayer::PlayerProjectile:
        return CollisionTarget::PlayerProjectile;
    case CollisionLayer::EnemyProjectile:
        return CollisionTarget::EnemyProjectile;
    case CollisionLayer::None:
    default:
        return CollisionTarget::None;
    }
}

[[nodiscard]] constexpr bool has_collision_target(
    CollisionTarget targets,
    CollisionLayer layer) noexcept
{
    const std::uint32_t target_bits = static_cast<std::uint32_t>(targets);
    const std::uint32_t layer_bit = static_cast<std::uint32_t>(
        to_collision_target(layer));

    return (target_bits & layer_bit) != 0U;
}

class CollisionBox;

struct CollisionInfo
{
    CollisionBox& self;
    CollisionBox& other;
};

using CollisionCallback = std::function<void(const CollisionInfo&)>;

class CollisionBox
{
public:
    explicit CollisionBox(
        GameObject* owner = nullptr,
        CollisionLayer layer = CollisionLayer::None,
        CollisionTarget targets = CollisionTarget::None) noexcept
        : _owner(owner),
          _layer(layer),
          _targets(targets)
    {
    }

    void set_owner(GameObject* owner) noexcept
    {
        _owner = owner;
    }

    void set_rect(const Rect& rect) noexcept
    {
        _rect = rect;
    }

    void set_layer(CollisionLayer layer) noexcept
    {
        _layer = layer;
    }

    void set_targets(CollisionTarget targets) noexcept
    {
        _targets = targets;
    }

    void set_on_collided(CollisionCallback on_collided)
    {
        _on_collided = std::move(on_collided);
    }

    [[nodiscard]] GameObject* owner() const noexcept
    {
        return _owner;
    }

    [[nodiscard]] const Rect& rect() const noexcept
    {
        return _rect;
    }

    [[nodiscard]] CollisionLayer layer() const noexcept
    {
        return _layer;
    }

    [[nodiscard]] CollisionTarget targets() const noexcept
    {
        return _targets;
    }

    [[nodiscard]] bool can_collide_with(const CollisionBox& other) const noexcept
    {
        return has_collision_target(_targets, other._layer)
            && has_collision_target(other._targets, _layer);
    }

    void invoke_on_collided(CollisionBox& other)
    {
        if (!_on_collided)
            return;

        _on_collided(CollisionInfo{ *this, other });
    }

private:
    Rect _rect{};
    GameObject* _owner = nullptr;
    CollisionLayer _layer = CollisionLayer::None;
    CollisionTarget _targets = CollisionTarget::None;
    CollisionCallback _on_collided;
};
