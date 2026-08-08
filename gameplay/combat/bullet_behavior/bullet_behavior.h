#pragma once

class BulletBehaviorContext;

class BulletBehavior
{
public:
    virtual ~BulletBehavior() = default;

    virtual void on_fire(BulletBehaviorContext &context) {}
    virtual void on_update(BulletBehaviorContext &context) {}
    virtual void on_death(BulletBehaviorContext &context) {}
    virtual bool on_collision(BulletBehaviorContext &context) { return false; }
    virtual bool on_entity_collision(BulletBehaviorContext &context) { return false; }
};