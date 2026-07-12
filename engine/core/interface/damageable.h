#pragma once

class GameObject;

struct DamageInfo
{
    float amount = 0.0f;
    GameObject* attacker = nullptr;
};

class Damageable
{
public:
    virtual ~Damageable() = default;

    virtual void take_damage(const DamageInfo& damage_info) noexcept = 0;
};
