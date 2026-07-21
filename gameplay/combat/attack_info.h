#pragma once

#include <memory>
#include <vector>

class StatusEffect;

struct AttackInfo
{
    float base_damage = 0.0f;
    std::vector<std::shared_ptr<StatusEffect>> effects;
};

class CombatReceiver
{
public:
    virtual ~CombatReceiver() = default;

    virtual void receive_attack(const AttackInfo &attack_info) noexcept = 0;
};
