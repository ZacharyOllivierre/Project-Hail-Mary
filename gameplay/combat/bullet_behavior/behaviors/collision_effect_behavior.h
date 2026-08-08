#pragma once

#include "../bullet_behavior_context.h"
#include "../bullet_behavior.h"

#include <string>

// Spawns a visual effect on collision
class CollisionEffectBehavior : public BulletBehavior
{
public:
    explicit CollisionEffectBehavior(std::string effect_key) : _effect_key(effect_key)
    {
    }

    bool on_collision(BulletBehaviorContext &context) override
    {
        spawn_effect(context);
        return false;
    }

    bool on_entity_collision(BulletBehaviorContext &context) override
    {
        spawn_effect(context);
        return false;
    }

private:
    void spawn_effect(BulletBehaviorContext &context);

private:
    std::string _effect_key;
};
