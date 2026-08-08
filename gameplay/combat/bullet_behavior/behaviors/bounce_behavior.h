#pragma once

#include "../bullet_behavior_context.h"
#include "../bullet_behavior.h"

class BounceBehavior : public BulletBehavior
{
public:
    explicit BounceBehavior(int bounces) : _remaining_bounces(bounces)
    {
    }

    bool on_collision(BulletBehaviorContext &context) override;

private:
    int _remaining_bounces;
};