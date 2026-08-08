#pragma once

#include "../bullet_behavior/bullet_behavior_set.h"
#include "../wand_types.h"

#include <functional>
#include <memory>
#include <vector>

class RuneLine;

struct RuneLoadout;

enum class RuneType
{
    Weapon,
    Stat,
    Behavior
};

enum class WeaponConsumeType
{
    // Merge the child weapon's modifiers into the parent loadout.
    AddModifiers,

    // Fire the child weapon as a nested shot branch.
    FireNested
};

// Note: a weapon may only consume one other weapon
struct WeaponConsumptionData
{
    // How this weapon consumes the runes that follow it.
    WeaponConsumeType consume_type = WeaponConsumeType::FireNested;

    // Number of following rune slots this weapon can consume.
    int consumed_runes = 0;

    // Delay to apply when firing a nested child weapon.
    float consumption_fire_interval_seconds = 0.0f;
};

struct RuneLoadout
{
    // The final weapon attributes produced by the consumed rune chain
    WandAttributes wand_attributes;
    Bullet_Attributes bullet_attributes;

    // Behaviors appended by behavior runes
    std::vector<std::function<void(BulletBehaviorSet &)>> bullet_behavior_appenders;
};

struct RuneWeaponNode
{
    int slot_index = 0;
    WeaponConsumptionData consumption;
    RuneLoadout loadout;
    std::vector<RuneWeaponNode> children;
};

class Rune
{
public:
    explicit Rune(RuneType type) : _type(type) {}

    virtual ~Rune() = default;

    RuneType type() const noexcept { return _type; }

    virtual void apply_weapon(RuneLoadout &loadout) const
    {
        (void)loadout;
    }

    virtual void apply_stat(RuneLoadout &loadout) const
    {
        (void)loadout;
    }

private:
    RuneType _type;
};
