#include "pierce_behavior.h"

#include "../../bullet.h"

bool PierceBehavior::on_entity_collision(BulletBehaviorContext &context)
{
    if (_pierces <= 0)
    {
        return false;
    }

    _pierces--;
    return true;
}