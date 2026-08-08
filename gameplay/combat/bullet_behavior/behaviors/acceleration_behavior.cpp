#include "acceleration_behavior.h"

#include "../../../../engine/core/geometry/vector2.h"
#include "../../bullet.h"

void AccelerationBehavior::on_update(BulletBehaviorContext &context)
{
    engine::core::Vector2 newVelocity = context.bullet.desired_velocity();
    engine::core::Vector2 direction = context.bullet.desired_velocity().normalized();
    newVelocity += direction * (_acceleration * context.delta);

    context.bullet.set_velocity(newVelocity);
}