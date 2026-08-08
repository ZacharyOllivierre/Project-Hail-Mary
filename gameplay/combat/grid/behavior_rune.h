#pragma once

#include "rune.h"

#include <functional>
#include <memory>

#include "../bullet_behavior/behaviors/pierce_behavior.h"
#include "../bullet_behavior/behaviors/bounce_behavior.h"

class BehaviorRune : public Rune
{
protected:
    BehaviorRune() : Rune(RuneType::Behavior) {}

public:
    void apply_weapon(RuneLoadout &loadout) const override
    {
        loadout.bullet_behavior_appenders.push_back(make_appender());
    }

protected:
    virtual std::function<void(BulletBehaviorSet &)> make_appender() const = 0;
};

class PierceRune : public BehaviorRune
{
public:
    explicit PierceRune(int pierces = 1) : _pierces(pierces) {}

protected:
    std::function<void(BulletBehaviorSet &)> make_appender() const override
    {
        int pierces = _pierces;
        return [pierces](BulletBehaviorSet &behavior_set)
        {
            behavior_set.add(std::make_unique<PierceBehavior>(pierces));
        };
    }

private:
    int _pierces = 1;
};

class BounceRune : public BehaviorRune
{
public:
    explicit BounceRune(int bounces = 1) : _bounces(bounces) {}

protected:
    std::function<void(BulletBehaviorSet &)> make_appender() const override
    {
        int bounces = _bounces;
        return [bounces](BulletBehaviorSet &behavior_set)
        {
            behavior_set.add(std::make_unique<BounceBehavior>(bounces));
        };
    }

private:
    int _bounces = 1;
};