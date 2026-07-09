#include "wand.h"

#include "bullet.h"

#include <utility>

std::unique_ptr<Projectile> Wand::attack(const Vector2 &origin, const Vector2 &direction) const
{
    if (direction.is_zero())
        return std::make_unique<Bullet>(origin, Vector2(_bullet_speed, 0.0f));

    return std::make_unique<Bullet>(
        origin,
        direction * _bullet_speed);
}