#pragma once

#include "bullet_behavior.h"
#include "bullet_behavior_context.h"

// Todo change name of file, is confusing with bullet behavior set

// Accelerates bullet during flight
class AccelerationBehavior : public BulletBehavior
{
public:
    explicit AccelerationBehavior(float acceleration) : _acceleration(acceleration)
    {
    }

    void on_update(BulletBehaviorContext &context) override;

private:
    float _acceleration = 0.0f;
};

// Bounces bullet on projectile structure collision
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

#include <string>
// Spawns a visual effect on collision
// This behavior does not have a matching behavior rune
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

class HomingBehavior : public BulletBehavior
{
public:
    explicit HomingBehavior(float strength, bool maintains_speed)
        : _strength(strength), _homing_maintains_speed(maintains_speed)
    {
    }

    void on_update(BulletBehaviorContext &context) override;

private:
    int _strength = 0;
    bool _homing_maintains_speed = true;
};

class PierceBehavior : public BulletBehavior
{
public:
    explicit PierceBehavior(int pierces) : _pierces(pierces)
    {
    }

    bool on_entity_collision(BulletBehaviorContext &context) override;

private:
    int _pierces = 0;
};