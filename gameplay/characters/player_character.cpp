#include "player_character.h"

#include "../../engine/animation/animation_manager.h"
#include "../../engine/core/render/render_command.h"
#include "../../engine/effects/effect_service.h"
#include "../../engine/input/input_state.h"
#include "../../engine/tools/logger.h"

#include "../../engine/physics/collision_manager.h"

#include "../combat/projectile_service.h"

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

    // move controller supplies temporary ability velocity normal movement continues after
    const bool was_ability_active = _movement_abilities.is_active();

    _movement_abilities.update(static_cast<float>(scaled_delta(delta)));

    if (_movement_abilities.is_active())
        set_desired_velocity(_movement_abilities.active_velocity());
    else if (was_ability_active)
        set_desired_velocity(engine::core::Vector2::zero());

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

    if (_movement_abilities.is_active())
        return;

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
        ENGINE_LOG_DEBUG("gameplay", "effect");
        engine::effects::AnimationEffectSpawnRequest request;
        request.effect_key = _effect_id;
        request.position = center();
        request.anchor = engine::effects::EffectAnchor::Center;
        request.size = engine::core::Vector2{200.0f, 200.0f};
        request.flip = _facing_direction == FacingDirection::Left
                           ? engine::core::SpriteFlip::Horizontal
                           : engine::core::SpriteFlip::None;
        if (!EFFECT_SERVICE->request_animation_effect(request))
        {
            ENGINE_LOG_DEBUG("gameplay", "false");
        }
    }
}

void PlayerCharacter::activate_movement_ability(
    MovementAbilityType type,
    const engine::core::Vector2 &target) noexcept
{
    if (is_dead())
        return;

    const MovementAbilityController::ActivationResult result = _movement_abilities.activate(
        type, target, center(), size(), movement_direction());
    if (!result.activated)
        return;

    if (result.teleported)
        set_position(result.destination - size() * 0.5f);
}

void PlayerCharacter::set_movement_bounds(const engine::core::Rect &bounds) noexcept
{
    _movement_abilities.set_bounds(bounds);
}

void PlayerCharacter::set_movement_ability(MovementAbilityType type) noexcept
{
    _movement_abilities.set_selected(type);
}

MovementAbilityType PlayerCharacter::movement_ability() const noexcept
{
    return _movement_abilities.selected();
}

engine::core::Vector2 PlayerCharacter::movement_direction() const noexcept
{
    const engine::core::Vector2 velocity = desired_velocity();
    if (!velocity.is_zero())
        return velocity.normalized();

    return _facing_direction == FacingDirection::Left
               ? engine::core::Vector2(-1.0f, 0.0f)
               : engine::core::Vector2(1.0f, 0.0f);
}

engine::core::Vector2 PlayerCharacter::teleport_destination(
    const engine::core::Vector2 &target) const noexcept
{
    return _movement_abilities.teleport_destination(target, center(), size());
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

void PlayerCharacter::create_projectile(const engine::core::Vector2 &direction)
{
    // mabey put requset in wand?
    ProjectileFireRequest requset;
    requset.source = this;
    requset.shots = _wand.attack(direction);
    requset.collision.layer = engine::physics::CollisionLayer::PlayerProjectile;
    requset.collision.targets = engine::physics::CollisionTarget::Enemy;
    ENGINE_LOG_DEBUG("PlayerCharacter", "create_projectile");

    PROJECTILE_SERVICE->request_fire(std::move(requset));
}

Wand &PlayerCharacter::wand() noexcept
{
    return _wand;
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
        ENGINE_LOG_WARN("gameplay", "Set PlayerCharacter animation failed: "
                                        << _character_id << "." << suffix);
}

engine::core::Rect PlayerCharacter::make_body_collision_rect(
    const engine::core::Rect &render_rect) const noexcept
{
    return make_bottom_centered_scaled_rect(render_rect, {0.65f, 0.38f});
}

void PlayerCharacter::on_died() noexcept
{
    set_animation_state(AnimationState::Die);
}
