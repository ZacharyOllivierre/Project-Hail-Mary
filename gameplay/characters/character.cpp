#include "character.h"

#include <algorithm>

Character::Character(const engine::core::Vector2& start_position, const engine::core::Vector2& start_size)
    : engine::core::GameObject(engine::core::DepthLayer::Character)
{
    set_position(start_position);
    set_character_size(start_size);
}

Character::~Character() = default;

void Character::set_move_speed(float move_speed) noexcept
{
    _move_speed = std::max(0.0f, move_speed);
}

void Character::set_hp(float hp) noexcept
{
    _hp = std::max(0.0f, hp);
    if (_hp <= 0.0f)
        die();
}

void Character::set_desired_velocity(const engine::core::Vector2& velocity) noexcept
{
    _desired_velocity = _is_dead ? engine::core::Vector2::zero() : velocity;
}

engine::core::Vector2 Character::desired_velocity() const noexcept
{
    return _desired_velocity;
}

void Character::die() noexcept
{
    if (_is_dead)
        return;

    _is_dead = true;
    _desired_velocity = engine::core::Vector2::zero();
}


engine::core::Rect Character::collision_rect() const noexcept
{
    return _collision_rect;
}

engine::core::Rect Character::hurt_rect() const noexcept
{
    return _hurt_rect;
}


float Character::move_speed() const noexcept
{
    return _move_speed;
}

float Character::hp() const noexcept
{
    return _hp;
}

bool Character::is_dead() const noexcept
{
    return _is_dead;
}
