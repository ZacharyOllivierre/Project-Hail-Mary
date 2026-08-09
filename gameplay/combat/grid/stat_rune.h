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

// Increases damage by amount
class DamageUpRune : public StatRune
{
public:
    explicit DamageUpRune(float damage_bonus = 0) : _damage_bonus(damage_bonus) {}

protected:
    void modify_loadout(RuneLoadout &loadout) const override
    {
        loadout.bullet_attributes.damage += _damage_bonus;
    }

private:
    float _damage_bonus = 10.0f;
};

// Increases or sets bullet count
class BulletCountUpRune : public StatRune
{
public:
    explicit BulletCountUpRune(int bullet_count = 0, bool add_to_total = true)
        : _bullet_count(bullet_count), _add_to_total(add_to_total) {}

protected:
    void modify_loadout(RuneLoadout &loadout) const override
    {
        if (_add_to_total)
        {
            loadout.wand_attributes.bullet_count += _bullet_count;
        }
        else
        {
            loadout.wand_attributes.bullet_count = _bullet_count;
        }
    }

private:
    int _bullet_count = 0;
    bool _add_to_total = true;
};

class SpreadStyleChangeRune : public StatRune
{
public:
    explicit SpreadStyleChangeRune(SpreadStyle spreadstyle)
        : _spread_style(spreadstyle) {}

protected:
    void modify_loadout(RuneLoadout &loadout) const override
    {
        loadout.wand_attributes.spread_style = _spread_style;
    }

private:
    SpreadStyle _spread_style;
};
