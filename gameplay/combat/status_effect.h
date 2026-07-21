#pragma once

#include <memory>

#include "attack_info.h"

class StatusEffect
{
public:
    virtual ~StatusEffect() = default;

    virtual std::shared_ptr<StatusEffect> make_new_instance() const = 0;
    virtual void on_apply(CombatReceiver &target) {}
    virtual void on_remove(CombatReceiver &target) {}
    virtual void update(CombatReceiver &target, double delta) = 0;
    virtual bool finished() const = 0;
};

class DamageOverTimeEffect
{
public:
    DamageOverTimeEffect(float duration, int ticks)
        : _duration_seconds(duration), _ticks(ticks)
    {
        if (_ticks > 0)
            _tick_interval = _duration_seconds / static_cast<float>(_ticks);

        else
            _tick_interval = 0.0f;
    }

    void update(double delta)
    {
        if (_duration_seconds > 0.0f)
            _elapsed_seconds += delta;

        if (_tick_interval > 0.0f)
            _elapsed_since_tick += delta;
    }

    bool should_damage()
    {
        if (_tick_interval <= 0.0f)
        {
            return false;
        }

        if (_elapsed_since_tick >= _tick_interval)
        {
            _elapsed_since_tick -= _tick_interval;
            return true;
        }

        return false;
    }

protected:
    bool expired() const
    {
        return _elapsed_seconds >= _duration_seconds;
    }

protected:
    float _duration_seconds = 0.0f;
    float _elapsed_seconds = 0.0f;

    // Consolodates damage into this many ticks accross duration
    int _ticks = 0;
    float _tick_interval = 0.0f;
    float _elapsed_since_tick = 0.0f;
};

class PoisonEffect : public StatusEffect, protected DamageOverTimeEffect
{
public:
    PoisonEffect(float dps, float duration, int ticks)
        : DamageOverTimeEffect(duration, ticks), _dps(dps) {}

    std::shared_ptr<StatusEffect> make_new_instance() const override
    {
        return std::make_shared<PoisonEffect>(_dps, DamageOverTimeEffect::_duration_seconds, DamageOverTimeEffect::_ticks);
    }

    void update(CombatReceiver &target, double delta) override
    {
        DamageOverTimeEffect::update(delta);

        if (DamageOverTimeEffect::should_damage())
        {
            // Deal tick of damage
            AttackInfo attack_info;
            attack_info.base_damage = _dps * _tick_interval;
            target.receive_attack(attack_info);
        }
    }

    bool finished() const override
    {
        return expired();
    }

private:
    float _dps = 0.0f;
};

// Todo Not currently possible as health isnt exposed through combat_reciever
// class BleedEffect : public StatusEffect, protected DamageOverTimeEffect
// {
// public:
//     BleedEffect(float percent, float duration, int ticks)
//         : DamageOverTimeEffect(duration, ticks), _percent(percent) {}

//     std::shared_ptr<StatusEffect> make_new_instance() const override
//     {
//         return std::make_shared<BleedEffect>(_percent, DamageOverTimeEffect::_duration_seconds, DamageOverTimeEffect::_ticks);
//     }

//     void update(CombatReceiver &target, double delta) override
//     {
//         DamageOverTimeEffect::update(delta);

//         if (DamageOverTimeEffect::should_damage())
//         {
//             // Deal tick of damage
//             AttackInfo attack_info;
//             attack_info.base_damage = target.hp() * _percent;
//             target.receive_attack(attack_info);

//         }
//     }

//     bool finished() const override
//     {
//         return expired();
//     }

// private:
//     float _percent;
// };