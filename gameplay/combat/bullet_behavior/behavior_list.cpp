#include "behavior_list.h"

#include "../../../engine/core/geometry/vector2.h"
#include "../bullet.h"

// Collision effect
#include "../../../engine/animation/effect_manager.h"
#include "../../../engine/scene/scene_manager.h"
#include "../../scene/room_scene.h"

#include "../../../engine/audio/audio_service.h"

void AccelerationBehavior::on_update(BulletBehaviorContext &context)
{
    engine::core::Vector2 new_velocity = context.bullet.desired_velocity();
    engine::core::Vector2 direction = context.bullet.desired_velocity().normalized();
    new_velocity += direction * (_acceleration * context.delta);

    context.bullet.set_velocity(new_velocity);
}

void DecelerationBehavior::on_update(BulletBehaviorContext &context)
{
    engine::core::Vector2 velocity = context.bullet.desired_velocity();
    float speed = velocity.length();

    // enforce min speed
    if (speed <= _min_speed)
    {
        return;
    }

    float new_speed = speed - (_deceleration * context.delta);
    new_speed = std::max(new_speed, _min_speed); // enforce min speed

    engine::core::Vector2 direction = velocity.normalized();

    context.bullet.set_velocity(new_speed * direction);
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

    // Call bounce sound effect
    engine::audio::SoundPlayOptions options{
        .group = engine::audio::SoundGroup::Wand,
        .loops = 0,
        .start_delay = std::chrono::milliseconds{0}};

    AUDIO_SERVICE->request_sound("bounce", options);

    return true;
}

// Todo this code doesnt make sense looking back
void CurveBehavior::on_update(BulletBehaviorContext &context)
{
    engine::core::Vector2 velocity = context.bullet.desired_velocity();

    // Bullets only curve when going a real speed
    float min_speed = 1.0f;
    if (velocity.length() < min_speed)
        return;

    engine::core::Vector2 forward = velocity.normalized();

    // Local left/right relative to travel direction.
    engine::core::Vector2 left = {-forward.y, forward.x};

    // Apply curve
    velocity += left * (_curve * (context.delta));

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
    // Bullets only home when going a real speed
    float min_speed = 1.0f;
    if (context.bullet.desired_velocity().length() < min_speed)
        return;

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

    // Call pierce sound effect
    engine::audio::SoundPlayOptions options{
        .group = engine::audio::SoundGroup::Wand,
        .loops = 0,
        .start_delay = std::chrono::milliseconds{0}};

    AUDIO_SERVICE->request_sound(
        "pierce",
        options);

    _pierces--;
    return true;
}

bool WallStickBehavior::on_collision(BulletBehaviorContext &context)
{
    if (_stick_length <= 0.0f)
        return false;

    _last_collision_direction = context.collision_direction;

    engine::core::Vector2 velocity =
        context.bullet.desired_velocity();

    float speed = velocity.length();

    engine::core::Vector2 wall_direction =
        -context.collision_direction.normalized();

    _stored_velocity = wall_direction * speed;

    context.bullet.set_velocity(wall_direction * 0.0001f);

    _stuck_to_wall = true;
    _elapsed_since_activation = 0.0f;

    return true;
}

void WallStickBehavior::on_update(BulletBehaviorContext &context)
{
    if (!_stuck_to_wall)
        return;

    _stick_length -= context.delta;
    _elapsed_since_activation += context.delta;

    // Activate collision effects
    if (_elapsed_since_activation >= _activation_interval)
    {
        _elapsed_since_activation = 0.0f;

        context.bullet.set_velocity(_stored_velocity);
        _stuck_to_wall = false;

        BulletBehaviorContext collision_context{
            .bullet = context.bullet,
            .collision_direction = _last_collision_direction,
            .delta = context.delta};

        context.bullet.behavior_set()->replay_collision_behaviors_except(
            collision_context,
            this);
    }

    // Wall stick ends
    if (_stick_length <= 0)
    {
        context.bullet.set_velocity(_stored_velocity);
        _stuck_to_wall = false;
    }
}
