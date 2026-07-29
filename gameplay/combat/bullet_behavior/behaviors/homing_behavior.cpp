#include "homing_behavior.h"

#include "../../bullet.h"
#include "../../../../engine/scene/scene_manager.h"
#include "../../../scene/room_scene.h"

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