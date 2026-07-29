#pragma once

#include "../bullet_behavior.h"
#include "../bullet_behavior_context.h"

class GrowthBehavior : public BulletBehavior
{
public:
    explicit GrowthBehavior(float growth) : _growth(growth)
    {
    }

    void on_update(BulletBehaviorContext &context) override;

private:
    float _growth = 0.0f;

    // First update recieved sets base damage for scaling
    float _base_damage = 0.0f;
};