#include "player_character.h"

#include "../../engine/animation/animation_manager.h"
#include "../../engine/core/render/render_command.h"
#include "../../engine/input/input_state.h"

#include "../../engine/physics/collision_manager.h"

#include <iostream>
#include <utility>

PlayerCharacter::PlayerCharacter(std::string character_id,
                                 const engine::core::Vector2 &start_position, const engine::core::Vector2 &start_size,
                                 std::string effect_id)
    : Character(start_position, start_size),
      _character_id(std::move(character_id)),
      _effect_id(std::move(effect_id))
{
    set_animation_state(AnimationState::Idle);

    // tmp testing
    auto box = engine::physics::CollisionManager::instance()->create_box(this, engine::physics::CollisionLayer::Player,
                                                                         engine::physics::CollisionTarget::EnemyProjectile, {});
    set_hurt_collision_box(box);
    refresh_collision_shapes();
    // tmp testing
}

void PlayerCharacter::update(double delta)
{
    Character::update(delta);

    if (_animation)
        _animation->update(scaled_delta(delta));
}

void PlayerCharacter::on_input_snapshot(const engine::input::InputSnapshot &input)
{
    if (is_dead())
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
    set_desired_velocity(
        movement.is_zero()
            ? engine::core::Vector2::zero()
            : movement.normalized() * move_speed());

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

engine::core::Rect PlayerCharacter::make_body_collision_rect(
    const engine::core::Rect& render_rect) const noexcept
{
    return make_bottom_centered_scaled_rect(render_rect, {0.65f, 0.38f});
}

void PlayerCharacter::on_died() noexcept
{
    set_animation_state(AnimationState::Die);
}
