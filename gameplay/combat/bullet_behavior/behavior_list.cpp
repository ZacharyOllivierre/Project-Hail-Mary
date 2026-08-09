#include "behavior_list.h"

#include "../../../engine/core/geometry/vector2.h"
#include "../bullet.h"

// Collision effect
#include "../../../engine/animation/effect_manager.h"
#include "../../../engine/scene/scene_manager.h"
#include "../../scene/room_scene.h"

void AccelerationBehavior::on_update(BulletBehaviorContext &context)
{
    engine::core::Vector2 newVelocity = context.bullet.desired_velocity();
    engine::core::Vector2 direction = context.bullet.desired_velocity().normalized();
    newVelocity += direction * (_acceleration * context.delta);

    context.bullet.set_velocity(newVelocity);
}

bool BounceBehavior::on_collision(BulletBehaviorContext &context)
{
    if (_remaining_bounces <= 0)
    {
        return false;
    }

    --_remaining_bounces;

    engine::core::Vector2 reflected_velocity = context.bullet.desired_velocity();

    if (std::fabs(context.collision_direction.x) > engine::core::Vector2::k_epsilon)
    {
        reflected_velocity.x = -reflected_velocity.x;
    }
    if (std::fabs(context.collision_direction.y) > engine::core::Vector2::k_epsilon)
    {
        reflected_velocity.y = -reflected_velocity.y;
    }

    context.bullet.set_velocity(reflected_velocity);

    return true;
}

// Redefinition (here and bullet.cpp)
constexpr double kRadiansToDegrees = 57.29577951308232;

void CollisionEffectBehavior::spawn_effect(BulletBehaviorContext &context)
{
    RoomScene *room_scene = engine::scene::SceneManager::instance()->try_find_scene<RoomScene>();

    if (!room_scene)
        return;

    engine::animation::EffectSpawnRequest request;
    request.effect_key = _effect_key;
    request.position = context.bullet.center();
    request.anchor = engine::animation::EffectAnchor::Center;

    // Angle effect opposite bullet degrees
    engine::core::Vector2 v = context.bullet.desired_velocity();
    request.angle_degrees = std::atan2(v.y, v.x) * kRadiansToDegrees + 180;

    room_scene->spawn_effect(request);
}

// Todo this code doesnt make sense looking back
void CurveBehavior::on_update(BulletBehaviorContext &context)
{
    engine::core::Vector2 velocity = context.bullet.desired_velocity();
    engine::core::Vector2 forward = velocity.normalized();

    // Local left/right relative to travel direction.
    engine::core::Vector2 left = {-forward.y, forward.x};

    // Apply curve
    velocity += left * (_curve * (context.delta));

    /*
    Todo this is a larger problem with how bullet keeps and
    updates two sources for stats:
    Bullet attributes
    Projectile velocity in base class
    Bullets update both but behaviors only effect projectile velocity

    Additionally behaviors will need to be able to access bullet attributes.
    */
    context.bullet.set_velocity(velocity);
}

void GrowthBehavior::on_update(BulletBehaviorContext &context)
{
    Bullet_Attributes *attributes = context.bullet.get_bullet_attributes();

    if (!_base_damage)
    {
        _base_damage = attributes->damage;
    }

    attributes->damage = _base_damage + _growth * context.bullet.age_seconds();
}

void HomingBehavior::on_update(BulletBehaviorContext &context)
{
    RoomScene *room_scene = engine::scene::SceneManager::instance()->try_find_scene<RoomScene>();
    if (!room_scene)
    {
        return;
    }

    float speed = context.bullet.desired_velocity().length();
    ;
    engine::core::Vector2 pos = context.bullet.center();
    engine::core::Vector2 target = room_scene->closest_enemy_to_point(pos);

    if (target.is_zero())
    {
        return;
    }

    // Represents desired velocity if perfectly pointing at enemy
    engine::core::Vector2 desired = (target - pos).normalized();
    desired *= context.bullet.get_bullet_attributes()->bullet_speed;

    engine::core::Vector2 velocity = context.bullet.desired_velocity();

    // Steering force required to point at target
    engine::core::Vector2 steering = desired - velocity;

    // Preform turn proportional to homing strength
    float maxTurn = _strength * context.delta;

    if (steering.length() > maxTurn)
    {
        steering = steering.normalized() * maxTurn;
    }
    velocity += steering;

    // Maintain speed
    if (_homing_maintains_speed)
        velocity = velocity.normalized() * speed;

    context.bullet.set_velocity(velocity);
}

bool PierceBehavior::on_entity_collision(BulletBehaviorContext &context)
{
    if (_pierces <= 0)
    {
        return false;
    }

    _pierces--;
    return true;
}