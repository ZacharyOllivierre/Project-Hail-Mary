#pragma once

#include "../bullet_behavior.h"
#include "../bullet_behavior_context.h"

class AccelerationBehavior : public BulletBehavior
{
public:
    explicit AccelerationBehavior(float acceleration) : _acceleration(acceleration)
    {
    }

    void on_update(BulletBehaviorContext &context) override;

private:
    float _acceleration = 0.0f;
};