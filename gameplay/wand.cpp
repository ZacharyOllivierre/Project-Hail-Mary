#include "wand.h"

std::unique_ptr<Projectile> Wand::attack(const Vector2 &origin, const Vector2 &direction)
{
    bullet_attributes.bullet_velocity = direction * bullet_attributes.bullet_speed;

    return std::make_unique<Bullet>(bullet_attributes, origin);
}