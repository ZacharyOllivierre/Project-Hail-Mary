#include "bullet_behavior_set.h"
#include "bullet_behavior_context.h"
#include "../bullet.h"

void BulletBehaviorSet::add(std::unique_ptr<BulletBehavior> behavior)
{
    _behaviors.push_back(std::move(behavior));
}

void BulletBehaviorSet::on_fire(BulletBehaviorContext &context)
{
    for (auto &behavior : _behaviors)
        behavior->on_fire(context);
}

void BulletBehaviorSet::on_update(BulletBehaviorContext &context)
{
    for (auto &behavior : _behaviors)
        behavior->on_update(context);
}

void BulletBehaviorSet::on_collision(BulletBehaviorContext &context)
{
    // Version that doesn't play rest of collision behaviors if one has handled it
    for (auto &behavior : _behaviors)
    {
        if (behavior->on_collision(context))
            return;
    }

    context.bullet.destroy();
}

/*
Plays collision behaviors for cases where a collision behavior triggers the effects of
other collision behaviors without re triggering itself.
Unlike on collision does not stop when behavior handles collision
*/
void BulletBehaviorSet::replay_collision_behaviors_except(BulletBehaviorContext &context,
                                                          BulletBehavior *skip_behavior)
{
    for (auto &behavior : _behaviors)
    {
        if (behavior.get() == skip_behavior)
            continue;

        behavior->on_collision(context);
    }
}

void BulletBehaviorSet::on_entity_collision(BulletBehaviorContext &context)
{
    bool handledCollision = false;

    for (auto &behavior : _behaviors)
    {
        if (behavior->on_entity_collision(context))
            handledCollision = true;
    }

    if (!handledCollision)
    {
        context.bullet.destroy();
    }
}

void BulletBehaviorSet::on_death(BulletBehaviorContext &context)
{
    for (auto &behavior : _behaviors)
        behavior->on_death(context);
}