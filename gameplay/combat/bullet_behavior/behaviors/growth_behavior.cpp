#include "growth_behavior.h"

#include "../../bullet.h"

void GrowthBehavior::on_update(BulletBehaviorContext &context)
{
    Bullet_Attributes *attributes = context.bullet.get_bullet_attributes();

    if (!_base_damage)
    {
        _base_damage = attributes->damage;
    }

    attributes->damage = _base_damage + _growth * context.bullet.age_seconds();
}