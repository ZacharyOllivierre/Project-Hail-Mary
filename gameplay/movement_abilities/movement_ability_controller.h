#pragma once

#include "abilities_types.h"
#include "../../engine/core/geometry/rect.h"

class MovementAbilityController
{
public:
    struct ActivationResult
    {
        bool activated = false;
        bool teleported = false;
        engine::core::Vector2 destination = engine::core::Vector2::zero();
    };

public:
    void update(float delta) noexcept;

    // TODO gross function
    [[nodiscard]] ActivationResult activate(
        MovementAbilityType type,
        const engine::core::Vector2 &target,
        const engine::core::Vector2 &current_center,
        const engine::core::Vector2 &character_size,
        const engine::core::Vector2 &movement_direction) noexcept;

    [[nodiscard]] MovementAbilityType selected() const noexcept;
    [[nodiscard]] bool is_active() const noexcept;
    [[nodiscard]] engine::core::Vector2 active_velocity() const noexcept;

    // TODO gross function
    [[nodiscard]] engine::core::Vector2 teleport_destination(
        const engine::core::Vector2 &target,
        const engine::core::Vector2 &current_center,
        const engine::core::Vector2 &character_size) const noexcept;

    void set_bounds(const engine::core::Rect &bounds) noexcept;
    void set_selected(MovementAbilityType type) noexcept;

private:
    struct State
    {
        // selection is persistent active describes the ability currently moving the player
        MovementAbilityType selected = MovementAbilityType::Teleport;
        MovementAbilityType active = MovementAbilityType::Teleport;
        engine::core::Vector2 direction = engine::core::Vector2::zero();

        float distance = 0.0f;
        float time_remaining = 0.0f;
        float cooldown_remaining = 0.0f;
    };

private:
    State _state;
    engine::core::Rect _bounds;
};