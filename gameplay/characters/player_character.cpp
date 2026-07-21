#include "player_character.h"

#include "../../engine/animation/animation_manager.h"
#include "../../engine/core/render/render_command.h"
#include "../../engine/input/input_state.h"

#include "../../engine/physics/collision_manager.h"

#include <algorithm>
#include <iostream>
#include <utility>

namespace
{
    [[nodiscard]] engine::core::Rect make_tile_collision_rect(const engine::core::Rect &render_rect) noexcept
    {
        engine::core::Rect rect = engine::core::Rect::zero();
        rect.set_size({render_rect.width() * 0.65f, render_rect.height() * 0.38f});
        rect.set_bottom_center(render_rect.bottom_center());
        return rect;
    }
}

PlayerCharacter::PlayerCharacter(std::string character_id,
                                 const engine::core::Vector2 &start_position, const engine::core::Vector2 &start_size,
                                 std::string effect_id)
    : _character_id(std::move(character_id)), _effect_id(std::move(effect_id))
{
    set_position(start_position);
    set_character_size(start_size);
    set_animation_state(AnimationState::Idle);

    // tmp testing
    auto box = engine::physics::CollisionManager::instance()->create_box(this, engine::physics::CollisionLayer::Player,
                                                                         engine::physics::CollisionTarget::EnemyProjectile, {});
    set_hurt_collision_box(box);
    // tmp testing
}

void PlayerCharacter::update(double delta)
{
    update_status_effects(delta);

    if (_animation)
        _animation->update(scaled_delta(delta));

    if (_is_dead)
        set_animation_state(AnimationState::Die);
}

void PlayerCharacter::on_input_snapshot(const engine::input::InputSnapshot &input)
{
    if (_is_dead)
        return;

    float move_x = 0.0f;
    float move_y = 0.0f;
    if (input.state.is_pressed(engine::input::InputAction::Left))
        move_x -= 1.0f;
    if (input.state.is_pressed(engine::input::InputAction::Right))
        move_x += 1.0f;
    if (input.state.is_pressed(engine::input::InputAction::Up))
        move_y -= 1.0f;
    if (input.state.is_pressed(engine::input::InputAction::Down))
        move_y += 1.0f;

    const engine::core::Vector2 movement{move_x, move_y};
    set_desired_velocity(movement.is_zero() ? engine::core::Vector2::zero() : movement.normalized() * _move_speed);

    if (movement.x < 0.0f)
        _facing_direction = FacingDirection::Left;
    if (movement.x > 0.0f)
        _facing_direction = FacingDirection::Right;
    set_animation_state(movement.is_zero() ? AnimationState::Idle : AnimationState::Move);

    if (input.state.is_just_pressed(engine::input::InputAction::Attack) && !_effect_id.empty())
    {
        _pending_effect_requests.push_back({.effect_key = _effect_id,
                                            .position = center(),
                                            .anchor = engine::animation::EffectAnchor::Center,
                                            .size = engine::core::Vector2{500.0f, 500.0f},
                                            .flip = _facing_direction == FacingDirection::Left
                                                        ? engine::core::SpriteFlip::Horizontal
                                                        : engine::core::SpriteFlip::None});
    }
}

void PlayerCharacter::submit_render_commands(std::vector<engine::core::RenderCommand> &out_commands) const
{
    if (!_animation)
        return;

    engine::core::RenderCommand command;
    if (_animation->build_render_command(
            world_rect(), 0.0,
            _facing_direction == FacingDirection::Left
                ? engine::core::SpriteFlip::Horizontal
                : engine::core::SpriteFlip::None,
            command))
        out_commands.push_back(std::move(command));
}

engine::core::Vector2 PlayerCharacter::desired_velocity() const noexcept { return _desired_velocity; }

void PlayerCharacter::apply_translation(const engine::core::Vector2 &delta) noexcept
{
    engine::core::GameObject::set_position(position() + delta);
    _collision_rect.set_position(_collision_rect.position() + delta);
    update_hurt_collision_box();
}

engine::core::Rect PlayerCharacter::collision_rect() const noexcept { return _collision_rect; }

void PlayerCharacter::receive_attack(const AttackInfo &attack_info) noexcept
{
    if (_is_dead)
        return;

    if (attack_info.base_damage > 0.0f)
        set_hp(_hp - attack_info.base_damage);

    for (const auto &effect : attack_info.effects)
    {
        add_status_effect(effect);
    }
}

void PlayerCharacter::die() noexcept
{
    if (_is_dead)
        return;

    _is_dead = true;
    _desired_velocity = engine::core::Vector2::zero();
    clear_status_effects();
    set_animation_state(AnimationState::Die);
}

void PlayerCharacter::set_move_speed(float move_speed) noexcept { _move_speed = std::max(0.0f, move_speed); }

void PlayerCharacter::set_hp(float hp) noexcept
{
    _hp = std::max(0.0f, hp);
    if (_hp <= 0.0f)
        die();
}

void PlayerCharacter::set_character_size(const engine::core::Vector2 &size)
{
    engine::core::GameObject::set_size(size);
    refresh_collision_rect();
}

void PlayerCharacter::set_position(const engine::core::Vector2 &position)
{
    engine::core::GameObject::set_position(position);
    refresh_collision_rect();
}

void PlayerCharacter::set_hurt_collision_box(engine::physics::CollisionBox *collision_box) noexcept
{
    _hurt_collision_box = collision_box;
    update_hurt_collision_box();
}

void PlayerCharacter::set_desired_velocity(const engine::core::Vector2 &velocity) noexcept
{
    _desired_velocity = _is_dead ? engine::core::Vector2::zero() : velocity;
}

float PlayerCharacter::move_speed() const noexcept { return _move_speed; }
float PlayerCharacter::hp() const noexcept { return _hp; }
bool PlayerCharacter::is_dead() const noexcept { return _is_dead; }

std::vector<ShotDescriptor> PlayerCharacter::create_projectile(const engine::core::Vector2 &direction)
{
    return _wand.attack(direction);
}

Wand &PlayerCharacter::wand() noexcept
{
    return _wand;
}

std::vector<engine::animation::EffectSpawnRequest> PlayerCharacter::drain_effect_spawn_requests()
{
    std::vector<engine::animation::EffectSpawnRequest> requests = std::move(_pending_effect_requests);
    _pending_effect_requests.clear();
    return requests;
}

void PlayerCharacter::set_animation_state(AnimationState state)
{
    if (_animation && _animation_state == state)
        return;

    const char *suffix = state == AnimationState::Move ? "move" : state == AnimationState::Die ? "die"
                                                                                               : "idle";
    _animation = engine::animation::AnimationManager::instance()->create_animation(_character_id + "." + suffix);
    if (_animation)
        _animation_state = state;
    else
        std::cout << "Set PlayerCharacter animation failed: " << _character_id << "." << suffix << '\n';
}

void PlayerCharacter::refresh_collision_rect()
{
    _collision_rect = make_tile_collision_rect(world_rect());
    update_hurt_collision_box();
}

void PlayerCharacter::update_hurt_collision_box() noexcept
{
    if (_hurt_collision_box)
        _hurt_collision_box->set_rect(world_rect());
}
