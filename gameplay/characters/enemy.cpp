#include "enemy.h"

#include "../../engine/animation/animation_manager.h"
#include "../../engine/core/render/render_command.h"

#include "../../engine/physics/collision_manager.h"

#include <algorithm>
#include <iostream>
#include <utility>

namespace
{
[[nodiscard]] engine::core::Rect make_tile_collision_rect(const engine::core::Rect& render_rect) noexcept
{
    engine::core::Rect rect = engine::core::Rect::zero();
    rect.set_size({render_rect.width() * 0.65f, render_rect.height() * 0.38f});
    rect.set_bottom_center(render_rect.bottom_center());
    return rect;
}
}

Enemy::Enemy(std::string character_id,
    const engine::core::Vector2& start_position,const engine::core::Vector2& start_size)
    : _character_id(std::move(character_id))
{
    set_position(start_position);
    set_character_size(start_size);
    set_animation_state(AnimationState::Idle);

    //tmp testing
    auto box = engine::physics::CollisionManager::instance()->create_box(this, engine::physics::CollisionLayer::Enemy,
        engine::physics::CollisionTarget::PlayerProjectile, [this](const engine::physics::CollisionInfo&) {
            set_hp(hp() - 50.0f);
        });
    set_hurt_collision_box(box);
    //tmp testing
}

void Enemy::update(double delta)
{
    if (_animation)
        _animation->update(scaled_delta(delta));
}

void Enemy::submit_render_commands(std::vector<engine::core::RenderCommand>& out_commands) const
{
    if (!_animation)
        return;

    engine::core::RenderCommand command;
    if (_animation->build_render_command(world_rect(), 0.0, engine::core::SpriteFlip::None, command))
        out_commands.push_back(std::move(command));
}

engine::core::Vector2 Enemy::desired_velocity() const noexcept { return _desired_velocity; }

void Enemy::apply_translation(const engine::core::Vector2& delta) noexcept
{
    engine::core::GameObject::set_position(position() + delta);
    _collision_rect.set_position(_collision_rect.position() + delta);
    update_hurt_collision_box();
}

engine::core::Rect Enemy::collision_rect() const noexcept { return _collision_rect; }

void Enemy::receive_attack(const AttackInfo& attack_info) noexcept
{
    if (!_is_dead && attack_info.base_damage > 0.0f)
        set_hp(_hp - attack_info.base_damage);
}

void Enemy::die() noexcept
{
    if (_is_dead)
        return;

    _is_dead = true;
    _desired_velocity = engine::core::Vector2::zero();
    set_animation_state(AnimationState::Die);
}

void Enemy::set_move_speed(float move_speed) noexcept { _move_speed = std::max(0.0f, move_speed); }

void Enemy::set_hp(float hp) noexcept
{
    _hp = std::max(0.0f, hp);
    if (_hp <= 0.0f)
        die();
}

void Enemy::set_character_size(const engine::core::Vector2& size)
{
    engine::core::GameObject::set_size(size);
    refresh_collision_rect();
}

void Enemy::set_position(const engine::core::Vector2& position)
{
    engine::core::GameObject::set_position(position);
    refresh_collision_rect();
}

void Enemy::set_hurt_collision_box(engine::physics::CollisionBox* collision_box) noexcept
{
    _hurt_collision_box = collision_box;
    update_hurt_collision_box();
}

void Enemy::set_desired_velocity(const engine::core::Vector2& velocity) noexcept
{
    _desired_velocity = _is_dead ? engine::core::Vector2::zero() : velocity;
}

float Enemy::move_speed() const noexcept { return _move_speed; }
float Enemy::hp() const noexcept { return _hp; }
bool Enemy::is_dead() const noexcept { return _is_dead; }

void Enemy::refresh_collision_rect()
{
    _collision_rect = make_tile_collision_rect(world_rect());
    update_hurt_collision_box();
}

void Enemy::update_hurt_collision_box() noexcept
{
    if (_hurt_collision_box)
        _hurt_collision_box->set_rect(world_rect());
}

void Enemy::set_animation_state(AnimationState state)
{
    if (_animation && _animation_state == state)
        return;

    const char* suffix = state == AnimationState::Die ? "die" : "idle";
    std::unique_ptr<engine::animation::Animation> animation =
        engine::animation::AnimationManager::instance()->create_animation(_character_id + "." + suffix);
    if (!animation)
    {
        std::cout << "Set Enemy animation failed: " << _character_id << "." << suffix << '\n';
        return;
    }

    _animation = std::move(animation);
    _animation_state = state;
}
