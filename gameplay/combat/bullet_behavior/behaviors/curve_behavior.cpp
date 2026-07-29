#include "curve_behavior.h"

#include "../../bullet.h"

// Todo this code doesnt make sense looking back
// Change so operations are understandable
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

    Additionally behaviors will need to be able to access bullet attributes
    */
    context.bullet.set_velocity(velocity);
}
