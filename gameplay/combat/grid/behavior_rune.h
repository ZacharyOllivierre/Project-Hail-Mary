#pragma once

#include "rune.h"

#include <functional>
#include <memory>

#include "../bullet_behavior/behavior_list.h"

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

class AccelerationRune : public BehaviorRune
{
public:
    explicit AccelerationRune(float acceleration = 0) : _acceleration(acceleration) {}

protected:
    std::function<void(BulletBehaviorSet &)> make_appender() const override
    {
        float acceleration = _acceleration;
        return [acceleration](BulletBehaviorSet &behavior_set)
        {
            behavior_set.add(std::make_unique<AccelerationBehavior>(acceleration));
        };
    }

private:
    float _acceleration = 0.0f;
};

class BounceRune : public BehaviorRune
{
public:
    explicit BounceRune(int bounces = 0) : _bounces(bounces) {}

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
    int _bounces = 0;
};

class CurveRune : public BehaviorRune
{
public:
    explicit CurveRune(float curve = 0)
        : _curve(curve) {}

protected:
    std::function<void(BulletBehaviorSet &)> make_appender() const override
    {
        float curve = _curve;
        return [curve](BulletBehaviorSet &behavior_set)
        {
            behavior_set.add(std::make_unique<CurveBehavior>(curve));
        };
    }

private:
    float _curve = 0.0f;
};

class GrowthRune : public BehaviorRune
{
public:
    explicit GrowthRune(float growth = 0) : _growth(growth) {}

protected:
    std::function<void(BulletBehaviorSet &)> make_appender() const override
    {
        float growth = _growth;
        return [growth](BulletBehaviorSet &behavior_set)
        {
            behavior_set.add(std::make_unique<GrowthBehavior>(growth));
        };
    }

private:
    float _growth = 0.0f;
};

class HomingRune : public BehaviorRune
{
public:
    explicit HomingRune(float strength = 0, bool maintains_speed = true)
        : _strength(strength), _maintains_speed(maintains_speed) {}

protected:
    std::function<void(BulletBehaviorSet &)> make_appender() const override
    {
        float strength = _strength;
        bool maintains_speed = _maintains_speed;
        return [strength, maintains_speed](BulletBehaviorSet &behavior_set)
        {
            behavior_set.add(std::make_unique<HomingBehavior>(strength, maintains_speed));
        };
    }

private:
    float _strength = 0;
    bool _maintains_speed = true;
};

class PierceRune : public BehaviorRune
{
public:
    explicit PierceRune(int pierces = 0) : _pierces(pierces) {}

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
    int _pierces = 0;
};