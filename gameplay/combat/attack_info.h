#pragma once

#include <variant>
#include <vector>

struct SlowEffect
{
    float speed_multiplier = 1.0f;
    double duration_seconds = 0.0;
};

struct PoisonEffect
{
    float damage_per_second = 0.0f;
    double duration_seconds = 0.0;
};

struct StunEffect
{
    double duration_seconds = 0.0;
};

using CombatEffect = std::variant<SlowEffect, PoisonEffect, StunEffect>;

struct AttackInfo
{
    float base_damage = 0.0f;
    std::vector<CombatEffect> effects;
};

class CombatReceiver
{
public:
    virtual ~CombatReceiver() = default;

    virtual void receive_attack(const AttackInfo& attack_info) noexcept = 0;
};
