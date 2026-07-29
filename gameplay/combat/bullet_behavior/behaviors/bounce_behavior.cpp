#include "bounce_behavior.h"

#include "../../bullet.h"

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