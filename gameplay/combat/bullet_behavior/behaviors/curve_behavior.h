#pragma once

#include "../bullet_behavior.h"
#include "../bullet_behavior_context.h"

class CurveBehavior : public BulletBehavior
{
public:
    explicit CurveBehavior(float curve) : _curve(curve)
    {
    }

    void on_update(BulletBehaviorContext &context) override;

private:
    float _curve = 0.0f;
};