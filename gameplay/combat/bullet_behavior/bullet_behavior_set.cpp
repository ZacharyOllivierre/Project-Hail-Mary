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
    bool handledCollision = false;

    for (auto &behavior : _behaviors)
    {
        if (behavior->on_collision(context))
            handledCollision = true;
    }

    // Default destory bullet if collision is not handled
    if (!handledCollision)
    {
        context.bullet.destroy();
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