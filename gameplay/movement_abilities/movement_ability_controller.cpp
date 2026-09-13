#include "movement_ability_controller.h"

#include <algorithm>

void MovementAbilityController::update(float delta) noexcept
{
    _state.cooldown_remaining = std::max(0.0f, _state.cooldown_remaining - delta);
    _state.time_remaining = std::max(0.0f, _state.time_remaining - delta);
}

MovementAbilityController::ActivationResult MovementAbilityController::activate(
    MovementAbilityType type,
    const engine::core::Vector2 &target,
    const engine::core::Vector2 &current_center,
    const engine::core::Vector2 &character_size,
    const engine::core::Vector2 &movement_direction) noexcept
{
    if (_state.cooldown_remaining > 0.0f || is_active())
        return {};

    const MovementAbilityConfig config = movement_ability_config(type);
    if (type == MovementAbilityType::Teleport)
    {
        // teleport is applied immediately, the scene validates its floor destination
        const engine::core::Vector2 destination = teleport_destination(target, current_center, character_size);
        const engine::core::Vector2 offset = destination - current_center;
        if (offset.is_zero())
            return {};

        _state.cooldown_remaining = config.cooldown;
        return {.activated = true, .teleported = true, .destination = destination};
    }

    if (movement_direction.is_zero() || config.distance <= 0.0f)
        return {};

    // roll and dash are velocity driven so the physics manager deals with walls
    _state.active = type;
    _state.direction = movement_direction;
    _state.distance = config.distance;
    _state.time_remaining = config.duration;
    _state.cooldown_remaining = config.cooldown;
    return {.activated = true};
}

void MovementAbilityController::set_bounds(const engine::core::Rect &bounds) noexcept
{
    _bounds = bounds;
}

void MovementAbilityController::set_selected(MovementAbilityType type) noexcept
{
    _state.selected = type;
}

MovementAbilityType MovementAbilityController::selected() const noexcept
{
    return _state.selected;
}

bool MovementAbilityController::is_active() const noexcept
{
    return _state.time_remaining > 0.0f;
}

engine::core::Vector2 MovementAbilityController::active_velocity() const noexcept
{
    const MovementAbilityConfig config = movement_ability_config(_state.active);

    if (config.duration <= 0.0f || !is_active())
        return engine::core::Vector2::zero();

    return _state.direction * (_state.distance / config.duration);
}

engine::core::Vector2 MovementAbilityController::teleport_destination(
    const engine::core::Vector2 &target,
    const engine::core::Vector2 &current_center,
    const engine::core::Vector2 &character_size) const noexcept
{
    if (_bounds.width() <= 0.0f || _bounds.height() <= 0.0f)
        return current_center;

    const engine::core::Vector2 half_size = character_size * 0.5f;

    const engine::core::Vector2 bounded_target{
        std::clamp(target.x, _bounds.left() + half_size.x,
                   _bounds.right() - half_size.x),
        std::clamp(target.y, _bounds.top() + half_size.y,
                   _bounds.bottom() - half_size.y)};

    const engine::core::Vector2 offset = bounded_target - current_center;

    if (offset.is_zero())
        return current_center;

    const float distance = std::min(
        movement_ability_config(
            MovementAbilityType::Teleport)
            .max_range,
        offset.length());

    return current_center + offset.normalized() * distance;
}
