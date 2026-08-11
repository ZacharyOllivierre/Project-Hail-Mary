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

// Decelerates bullet during flight to a min if provided
class DecelerationBehavior : public BulletBehavior
{
public:
    explicit DecelerationBehavior(float deceleration = 0.0f, float min_speed = 0.0f)
        : _deceleration(deceleration), _min_speed(min_speed)
    {
    }

    void on_update(BulletBehaviorContext &context) override;

private:
    float _deceleration = 0.0f;
    float _min_speed = 0.0f;
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

// Curves bullet -val for left +val for right
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

// Grows bullet damage during flight
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

// Bullet homes to nearest enemy
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

// Bullets continue flight after entity collision
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

// Bullets stick to walls and repeat on collision effects on interval
class WallStickBehavior : public BulletBehavior
{
public:
    explicit WallStickBehavior(float stick_length = 0.0f, float activation_interval = 0.0f)
        : _stick_length(stick_length), _activation_interval(activation_interval)
    {
    }

    bool on_collision(BulletBehaviorContext &context) override;

    void on_update(BulletBehaviorContext &context) override;

private:
    bool _stuck_to_wall = false;
    engine::core::Vector2 _last_collision_direction = engine::core::Vector2::zero();

    // How long bullet sticks for
    float _stick_length = 0.0f;

    // How long between collision behaviors activations
    float _activation_interval = 0.0f;
    float _elapsed_since_activation = 0.0f;

    // Velocity of bullet before freeze
    engine::core::Vector2 _stored_velocity;
};
