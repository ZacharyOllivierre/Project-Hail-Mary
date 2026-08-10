#include "bullet.h"

#include "../../engine/core/render/render_command.h"
#include "../../engine/resources/resource_manager.h"
#include "../../engine/scene/scene_manager.h"
#include "../scene/room_scene.h"
#include "bullet_behavior/bullet_behavior_context.h"
#include "../../engine/audio/audio_service.h"

constexpr double kRadiansToDegrees = 57.29577951308232;

Bullet::Bullet(const Bullet_Attributes &bullet_attributes) noexcept
    : Projectile(
          // Set as item so spawns below the character to avoid overlap
          engine::core::DepthLayer::Item,
          bullet_attributes.start_position,
          bullet_attributes.bullet_size,
          bullet_attributes.bullet_velocity)
{
    _texture = engine::resources::ResourceManager::instance()->find_texture("bullet");

    _bullet_attributes = bullet_attributes;

    for (const std::function<void(BulletBehaviorSet &)> &append_behavior : _bullet_attributes.bullet_behavior_appenders)
    {
        append_behavior(_behaviors);
    }

    // Original damage of bullet before flight
    _base_damage = _bullet_attributes.damage;

    // Call on fire behaviors
    BulletBehaviorContext context{.bullet = *this};
    _behaviors.on_fire(context);

    // Call on fire sound if given
    if (!_bullet_attributes.sound_on_fire.empty())
    {
        AUDIO_SERVICE->play_sound(_bullet_attributes.sound_on_fire);
    }
}

// Todo collision box doesnt align with texture rotation
void Bullet::submit_render_commands(std::vector<engine::core::RenderCommand> &out_commands) const
{
    if (!_texture)
        return;

    engine::core::RenderCommand command;
    command.texture = _texture;
    command.command_rect = world_rect();
    const engine::core::Vector2 shot_velocity = desired_velocity();
    if (!shot_velocity.is_zero())
        command.rotation_degrees = std::atan2(shot_velocity.y, shot_velocity.x) * kRadiansToDegrees;
    out_commands.push_back(std::move(command));
}

// Collision direction represented as {-1 -> 1 , -1 -> 1}
void Bullet::on_collision(const engine::core::Vector2 &collision_direction) noexcept
{
    // Call on collision behaviors with context
    BulletBehaviorContext context{
        .bullet = *this,
        .collision_direction = collision_direction};

    _behaviors.on_collision(context);

    // Play structure collision sound
    if (!_bullet_attributes.sound_on_collision.empty())
    {
        AUDIO_SERVICE->play_sound(_bullet_attributes.sound_on_collision);
    }
}

void Bullet::on_entity_collision(GameObject *entity) noexcept
{
    // Adds entity to hit tracker to prevent multiples of damage
    _hit_cooldowns[entity] = _bullet_attributes.damage_cooldown_sec;

    // call on entity behaviors
    BulletBehaviorContext context{.bullet = *this, .entity = entity};
    _behaviors.on_entity_collision(context);

    // Play entity collision sound
    if (!_bullet_attributes.sound_on_collision.empty())
    {
        AUDIO_SERVICE->play_sound(_bullet_attributes.sound_on_entity_collision);
    }
}

void Bullet::on_destroy() noexcept
{
    // Call on destroy behaviors
    BulletBehaviorContext context{.bullet = *this};
    _behaviors.on_death(context);

    // Play projectile death sound
    if (!_bullet_attributes.sound_on_collision.empty())
    {
        AUDIO_SERVICE->play_sound(_bullet_attributes.sound_on_death);
    }
}

void Bullet::update(double delta)
{
    // Update age
    double age = Projectile::age_seconds();
    Projectile::set_age(age + delta);

    // Update hit cooldowns
    update_hit_cooldowns(delta);

    // Enforce max age
    if (age > _bullet_attributes.max_age)
    {
        Projectile::destroy();
    }

    // call on update behaviors
    BulletBehaviorContext context{.bullet = *this, .delta = delta};
    _behaviors.on_update(context);
}

AttackInfo Bullet::attack_info() const noexcept
{
    AttackInfo attack_info;
    attack_info.base_damage = _bullet_attributes.damage;

    if (_bullet_attributes.status_effect)
    {
        attack_info.effects.push_back(_bullet_attributes.status_effect);
    }

    return attack_info;
}

bool Bullet::can_hit(GameObject *object) const noexcept
{
    return !_hit_cooldowns.contains(object);
}

// Remove enemies with elapsed hit cooldowns off list
void Bullet::update_hit_cooldowns(double &delta)
{
    for (auto it = _hit_cooldowns.begin(); it != _hit_cooldowns.end();)
    {
        it->second -= delta;

        if (it->second <= 0)
        {
            it = _hit_cooldowns.erase(it);
        }
        else
            it++;
    }
}
