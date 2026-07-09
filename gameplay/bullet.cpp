#include "bullet.h"

#include "../engine/animation/effect_manager.h"
#include "../engine/core/render/render_command.h"
#include "../engine/scene/scene_manager.h"
#include "../engine/resources/resource_manager.h"
#include "scene/room_scene.h"

#include <cmath>

constexpr double kRadiansToDegrees = 57.29577951308232;

Bullet::Bullet(const Vector2 &start_position, const Vector2 &start_velocity) noexcept
    : Projectile(
          // Set as item so spawns below the character to avoid overlap
          DepthLayer::Item,
          Vector2::zero(),
          Vector2(24.0f, 24.0f),
          start_velocity)
{
    set_center(start_position);

    _texture = ResourceManager::instance()->find_texture("bullet");
}

void Bullet::submit_render_commands(std::vector<RenderCommand> &out_commands) const
{
    if (!_texture)
        return;

    RenderCommand command;
    command.texture = _texture;
    command.command_rect = world_rect();
    const Vector2 shot_velocity = desired_velocity();
    if (!shot_velocity.is_zero())
        command.rotation_degrees = std::atan2(shot_velocity.y, shot_velocity.x) * kRadiansToDegrees;
    out_commands.push_back(std::move(command));
}

void Bullet::on_collision() noexcept
{
    if (RoomScene *room_scene = SceneManager::instance()->try_find_scene<RoomScene>())
    {
        EffectSpawnRequest request;
        request.effect_key = "fire.impact_radial";
        request.position = center();
        request.anchor = EffectAnchor::Center;
        room_scene->spawn_effect(request);
    }

    Projectile::on_collision();
}