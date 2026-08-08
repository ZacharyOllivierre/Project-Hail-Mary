#pragma once

#include "rune.h"

class StatRune : public Rune
{
protected:
    StatRune() : Rune(RuneType::Stat) {}

public:
    void apply_stat(RuneLoadout &loadout) const override
    {
        modify_loadout(loadout);
    }

protected:
    virtual void modify_loadout(RuneLoadout &loadout) const = 0;
};

class DamageRune : public StatRune
{
public:
    explicit DamageRune(float damage_bonus = 10.0f) : _damage_bonus(damage_bonus) {}

protected:
    void modify_loadout(RuneLoadout &loadout) const override
    {
        loadout.bullet_attributes.damage += _damage_bonus;
    }

private:
    float _damage_bonus = 10.0f;
};