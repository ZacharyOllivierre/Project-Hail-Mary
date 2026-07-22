#include "enemy.h"

#include "../../engine/animation/animation_manager.h"
#include "../../engine/core/render/render_command.h"
#include "../../engine/physics/collision_manager.h"

#include <iostream>
#include <utility>

Enemy::Enemy(
    std::string character_id,
    const engine::core::Vector2& start_position,
    const engine::core::Vector2& start_size)
    : Character(start_position, start_size),
      _character_id(std::move(character_id))
{
    set_animation_state(AnimationState::Idle);

    engine::physics::CollisionBox* box =
        engine::physics::CollisionManager::instance()->create_box(
            this,
            engine::physics::CollisionLayer::Enemy,
            engine::physics::CollisionTarget::PlayerProjectile,
            {});
    set_hurt_collision_box(box);
    refresh_collision_shapes();
}

void Enemy::update(double delta)
{
    Character::update(delta);

    if (_animation)
        _animation->update(scaled_delta(delta));
}

void Enemy::submit_render_commands(
    std::vector<engine::core::RenderCommand>& out_commands) const
{
    if (!_animation)
        return;

    engine::core::RenderCommand command;
    if (_animation->build_render_command(
            world_rect(), 0.0, engine::core::SpriteFlip::None, command))
    {
        out_commands.push_back(std::move(command));
    }
}

engine::core::Rect Enemy::make_body_collision_rect(
    const engine::core::Rect& render_rect) const noexcept
{
    return make_bottom_centered_scaled_rect(render_rect, {0.65f, 0.38f});
}

void Enemy::on_died() noexcept
{
    set_animation_state(AnimationState::Die);
}

void Enemy::set_animation_state(AnimationState state)
{
    if (_animation && _animation_state == state)
        return;

    const char* suffix = state == AnimationState::Die ? "die" : "idle";
    std::unique_ptr<engine::animation::Animation> animation =
        engine::animation::AnimationManager::instance()->create_animation(
            _character_id + "." + suffix);
    if (!animation)
    {
        std::cout << "Set Enemy animation failed: "
                  << _character_id << "." << suffix << '\n';
        return;
    }

    _animation = std::move(animation);
    _animation_state = state;
}
