#include "character.h"

#include "../../engine/physics/collision_box.h"

#include <algorithm>

Character::Character(
    const engine::core::Vector2& start_position,
    const engine::core::Vector2& start_size) noexcept
    : engine::core::GameObject(engine::core::DepthLayer::Character)
{
    engine::core::GameObject::set_position(start_position);
    engine::core::GameObject::set_size(start_size);
}

void Character::update(double delta)
{
    _status_effect_manager.update(*this, delta);
}

engine::core::Vector2 Character::desired_velocity() const noexcept
{
    return _desired_velocity;
}

void Character::apply_translation(const engine::core::Vector2& delta) noexcept
{
    engine::core::GameObject::set_position(position() + delta);
    refresh_collision_shapes();
}

engine::core::Rect Character::collision_rect() const noexcept
{
    return _collision_rect;
}

void Character::receive_attack(const AttackInfo& attack_info) noexcept
{
    if (_is_dead)
        return;

    if (attack_info.base_damage > 0.0f)
        set_hp(_hp - attack_info.base_damage);

    for (const std::shared_ptr<StatusEffect>& effect : attack_info.effects)
        add_status_effect(effect);
}

void Character::die() noexcept
{
    if (_is_dead)
        return;

    _is_dead = true;
    _desired_velocity = engine::core::Vector2::zero();
    clear_status_effects();
    on_died();
}

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

void Character::set_character_size(const engine::core::Vector2& size)
{
    engine::core::GameObject::set_size(size);
    refresh_collision_shapes();
}

void Character::set_position(const engine::core::Vector2& position)
{
    engine::core::GameObject::set_position(position);
    refresh_collision_shapes();
}

void Character::set_desired_velocity(const engine::core::Vector2& velocity) noexcept
{
    _desired_velocity = _is_dead ? engine::core::Vector2::zero() : velocity;
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

void Character::set_hurt_collision_box(engine::physics::CollisionBox* collision_box) noexcept
{
    _hurt_collision_box = collision_box;
    refresh_collision_shapes();
}

void Character::refresh_collision_shapes() noexcept
{
    _collision_rect = make_body_collision_rect(world_rect());
    if (_hurt_collision_box)
        _hurt_collision_box->set_rect(make_hurt_box_rect(world_rect()));
}

engine::core::Rect Character::make_hurt_box_rect(
    const engine::core::Rect& render_rect) const noexcept
{
    return render_rect;
}

engine::core::Rect Character::make_bottom_centered_scaled_rect(
    const engine::core::Rect& source,
    const engine::core::Vector2& scale) noexcept
{
    engine::core::Rect result;
    result.set_size({source.width() * scale.x, source.height() * scale.y});
    result.set_bottom_center(source.bottom_center());
    return result;
}

engine::core::Rect Character::make_centered_scaled_rect(
    const engine::core::Rect& source,
    const engine::core::Vector2& scale) noexcept
{
    return engine::core::Rect::from_center(
        source.center(),
        {source.width() * scale.x, source.height() * scale.y});
}

void Character::add_status_effect(const std::shared_ptr<StatusEffect>& effect) noexcept
{
    if (effect)
        _status_effect_manager.add_effect(effect, *this);
}

void Character::clear_status_effects() noexcept
{
    _status_effect_manager.clear();
}
