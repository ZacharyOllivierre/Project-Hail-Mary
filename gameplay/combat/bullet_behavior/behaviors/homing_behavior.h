#pragma once

#include "../bullet_behavior.h"
#include "../bullet_behavior_context.h"

class HomingBehavior : public BulletBehavior
{
public:
    explicit HomingBehavior(float strength, bool maintains_speed)
        : _strength(strength), _homing_maintains_speed(maintains_speed)
    {
    }

    void on_update(BulletBehaviorContext &context) override;

private:
    int _strength = 0;
    bool _homing_maintains_speed = true;
};
