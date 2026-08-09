#pragma once

#include "bullet_behavior.h"

#include <memory>
#include <vector>

class BulletBehaviorSet
{
public:
    void add(std::unique_ptr<BulletBehavior> behavior);

    void on_fire(BulletBehaviorContext &context);

    void on_update(BulletBehaviorContext &context);

    void on_collision(BulletBehaviorContext &context);
    void replay_collision_behaviors_except(BulletBehaviorContext &context, BulletBehavior *skip_behavior);

    void on_entity_collision(BulletBehaviorContext &context);

    void on_death(BulletBehaviorContext &context);

private:
    std::vector<std::unique_ptr<BulletBehavior>> _behaviors;
};