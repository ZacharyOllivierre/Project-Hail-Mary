#include "bullet.h"

#include "../../engine/core/render/render_command.h"
#include "../../engine/resources/resource_manager.h"
#include "../../engine/scene/scene_manager.h"
#include "../scene/room_scene.h"

#include <cmath>

// Used to rotate texture by bullet velocity
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

    // Original damage of bullet before flight
    _base_damage = _bullet_attributes.damage;
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
    // Get scene and request collision effect
    // Todo save a pointer so dont have to search for scene each time
    RoomScene *room_scene = engine::scene::SceneManager::instance()->try_find_scene<RoomScene>();
    if (room_scene)
    {
        engine::animation::EffectSpawnRequest request = create_collision_effect("poison.explotion");
        room_scene->spawn_effect(request);
    }

    // Wall bounce
    bool bounced = handle_wall_bounce(collision_direction);

    // Destroy projectile if not bounced
    if (!bounced)
    {
        Projectile::on_collision(collision_direction);
    }
}

void Bullet::update(double delta)
{
    // Update age
    double age = Projectile::age_seconds();
    Projectile::set_age(age + delta);

    // Enforce max age
    if (age > _bullet_attributes.max_age)
    {
        Projectile::destroy();
    }
    // Apply curve
    if (_bullet_attributes.curve != 0)
    {
        apply_curve(delta);
    }
    // Appy growth
    if (_bullet_attributes.growth != 0)
    {
        apply_growth();
    }
    // Apply damage based sizing
    if (_bullet_attributes.damage_based_size)
    {
        apply_damage_sizing();
    }
    // Apply homing
    if (_bullet_attributes.homing_strength != 0)
    {
        apply_homing(delta);
    }
    // Apply accelleration
    if (_bullet_attributes.acceleration != 0)
    {
        apply_acceleration(delta);
    }
}

// Returns true if ball has been bounces otherwise false
bool Bullet::handle_wall_bounce(const engine::core::Vector2 &collision_direction)
{
    if (_bullet_attributes.bounces > 0)
    {
        _bullet_attributes.bounces--;

        engine::core::Vector2 reflected_velocity = Projectile::desired_velocity();
        if (std::fabs(collision_direction.x) > engine::core::Vector2::k_epsilon)
        {
            reflected_velocity.x = -reflected_velocity.x;
        }
        if (std::fabs(collision_direction.y) > engine::core::Vector2::k_epsilon)
        {
            reflected_velocity.y = -reflected_velocity.y;
        }

        _bullet_attributes.bullet_velocity = reflected_velocity;
        Projectile::set_velocity(_bullet_attributes.bullet_velocity);
        return true;
    }
    return false;
}

engine::animation::EffectSpawnRequest Bullet::create_collision_effect(const std::string &effect_key)
{
    engine::animation::EffectSpawnRequest request;
    request.effect_key = effect_key;
    request.position = engine::core::GameObject::center();
    request.anchor = engine::animation::EffectAnchor::Center;

    // Angle effect opposite bullet degrees
    engine::core::Vector2 v = _bullet_attributes.bullet_velocity;
    request.angle_degrees = std::atan2(v.y, v.x) * kRadiansToDegrees + 180;

    return request;
}

void Bullet::apply_curve(double &delta)
{
    engine::core::Vector2 velocity = _bullet_attributes.bullet_velocity;
    engine::core::Vector2 forward = velocity.normalized();

    // Local left/right relative to travel direction.
    engine::core::Vector2 left = {-forward.y, forward.x};

    velocity += left * (_bullet_attributes.curve * static_cast<float>(delta));

    set_velocity(velocity);
    _bullet_attributes.bullet_velocity = velocity;
}

void Bullet::apply_growth()
{
    _bullet_attributes.damage =
        _base_damage + _bullet_attributes.growth * Projectile::age_seconds();
}

void Bullet::apply_damage_sizing()
{
    engine::core::Vector2 new_size =
        _bullet_attributes.damage / _base_damage * _bullet_attributes.bullet_size;
    new_size += engine::core::Vector2(2, 2);

    Projectile::set_size(new_size);
}

void Bullet::apply_homing(double &delta)
{
    RoomScene *room_scene = engine::scene::SceneManager::instance()->try_find_scene<RoomScene>();
    if (!room_scene)
    {
        return;
    }

    float speed = _bullet_attributes.bullet_velocity.length();
    engine::core::Vector2 pos = center();
    engine::core::Vector2 target = room_scene->closest_enemy_to_point(pos);
    if (target.is_zero())
    {
        return;
    }

    // Represents desired velocity if perfectly pointing at enemy
    engine::core::Vector2 desired = (target - pos).normalized();
    desired *= _bullet_attributes.bullet_speed;

    engine::core::Vector2 velocity = Projectile::desired_velocity();

    // Steering force required to point at target
    engine::core::Vector2 steering = desired - velocity;

    // Preform turn proportional to homing strength
    float maxTurn = _bullet_attributes.homing_strength * delta;
    if (steering.length() > maxTurn)
    {
        steering = steering.normalized() * maxTurn;
    }
    velocity += steering;

    // Maintain speed
    if (_bullet_attributes.homing_maintains_speed)
        velocity = velocity.normalized() * speed;

    Projectile::set_velocity(velocity);
    _bullet_attributes.bullet_velocity = velocity;
}

void Bullet::apply_acceleration(double &delta)
{
    engine::core::Vector2 direction = _bullet_attributes.bullet_velocity.normalized();
    _bullet_attributes.bullet_velocity += direction * (_bullet_attributes.acceleration * delta);

    Projectile::set_velocity(_bullet_attributes.bullet_velocity);
}