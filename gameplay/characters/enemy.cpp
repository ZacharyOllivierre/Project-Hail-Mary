#include "enemy.h"

#include "../../engine/animation/animation_manager.h"
#include "../../engine/core/render/render_command.h"

#include <utility>

Enemy::Enemy(
    std::string character_id,
    const engine::core::Vector2& start_position,
    const engine::core::Vector2& start_size)
    : Character(start_position, start_size),
      _idle_animation(engine::animation::AnimationManager::instance()->create_animation(
          std::move(character_id) + ".idle"))
{
}

void Enemy::update(double delta)
{
    Character::update(delta);
    if (_idle_animation)
        _idle_animation->update(scaled_delta(delta));
}

void Enemy::submit_render_commands(std::vector<engine::core::RenderCommand>& out_commands) const
{
    if (!_idle_animation)
        return;

    engine::core::RenderCommand command;
    if (_idle_animation->build_render_command(world_rect(), 0.0, engine::core::SpriteFlip::None, command))
        out_commands.push_back(std::move(command));
}
