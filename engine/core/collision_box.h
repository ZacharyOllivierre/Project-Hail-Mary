#pragma once

#include "geometry/rect.h"

#include <cstdint>
#include <functional>
#include <utility>

class GameObject;

enum class CollisionSource : std::uint32_t
{
    None = 0,
    Player,
    Enemy,
    PlayerProjectile,
    EnemyProjectile
};

enum class CollisionTarget : std::uint32_t
{
    None = 0,
    Player = 1U << 0,
    Enemy = 1U << 1,
    PlayerProjectile = 1U << 2,
    EnemyProjectile = 1U << 3,
    All = (1U << 0) | (1U << 1) | (1U << 2) | (1U << 3)
};

[[nodiscard]] constexpr CollisionTarget to_collision_target(
    CollisionSource source) noexcept
{
    switch (source)
    {
    case CollisionSource::Player:
        return CollisionTarget::Player;
    case CollisionSource::Enemy:
        return CollisionTarget::Enemy;
    case CollisionSource::PlayerProjectile:
        return CollisionTarget::PlayerProjectile;
    case CollisionSource::EnemyProjectile:
        return CollisionTarget::EnemyProjectile;
    case CollisionSource::None:
    default:
        return CollisionTarget::None;
    }
}

[[nodiscard]] constexpr bool has_collision_target(
    CollisionTarget targets,
    CollisionSource source) noexcept
{
    const std::uint32_t target_bits = static_cast<std::uint32_t>(targets);
    const std::uint32_t source_bit = static_cast<std::uint32_t>(
        to_collision_target(source));

    return (target_bits & source_bit) != 0U;
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
        CollisionSource source = CollisionSource::None,
        CollisionTarget targets = CollisionTarget::None) noexcept
        : _owner(owner),
          _source(source),
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

    void set_source(CollisionSource source) noexcept
    {
        _source = source;
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

    [[nodiscard]] CollisionSource source() const noexcept
    {
        return _source;
    }

    [[nodiscard]] CollisionTarget targets() const noexcept
    {
        return _targets;
    }

    [[nodiscard]] bool can_collide_with(const CollisionBox& other) const noexcept
    {
        return has_collision_target(_targets, other._source)
            && has_collision_target(other._targets, _source);
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
    CollisionSource _source = CollisionSource::None;
    CollisionTarget _targets = CollisionTarget::None;
    CollisionCallback _on_collided;
};
