#pragma once

#include "../bullet_behavior.h"
#include "../bullet_behavior_context.h"

class PierceBehavior : public BulletBehavior
{
public:
    explicit PierceBehavior(int pierces) : _pierces(pierces)
    {
    }

    bool on_entity_collision(BulletBehaviorContext &context) override;

private:
    int _pierces = 0;
};