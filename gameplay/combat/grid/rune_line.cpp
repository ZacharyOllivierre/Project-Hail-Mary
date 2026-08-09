#include "rune_line.h"

#include "behavior_rune.h"
#include "stat_rune.h"
#include "weapon_rune.h"

#include <algorithm>

// TODO added slots not working (delete comment somewhere else).
namespace
{
    // merge child modifiers into the parent loadout without overwriting the parent weapon's base state.
    void merge_loadout(RuneLoadout &target, const RuneLoadout &source)
    {
        target.wand_attributes.bullet_count = std::max(target.wand_attributes.bullet_count, source.wand_attributes.bullet_count);
        target.wand_attributes.spawn_distance = std::max(target.wand_attributes.spawn_distance, source.wand_attributes.spawn_distance);
        target.wand_attributes.shot_delay_sec = std::max(target.wand_attributes.shot_delay_sec, source.wand_attributes.shot_delay_sec);
        target.wand_attributes.effect_chance = std::max(target.wand_attributes.effect_chance, source.wand_attributes.effect_chance);
        target.bullet_attributes.bullet_speed = std::max(target.bullet_attributes.bullet_speed, source.bullet_attributes.bullet_speed);
        target.bullet_attributes.max_age = std::max(target.bullet_attributes.max_age, source.bullet_attributes.max_age);
        target.bullet_attributes.damage += source.bullet_attributes.damage;
        target.bullet_behavior_appenders.insert(
            target.bullet_behavior_appenders.end(),
            source.bullet_behavior_appenders.begin(),
            source.bullet_behavior_appenders.end());
    }
}

RuneLine::RuneLine(int starting_slot_count)
    : _runes(starting_slot_count > 0 ? starting_slot_count : 1)
{
}

int RuneLine::slot_count() const noexcept
{
    return static_cast<int>(_runes.size());
}

bool RuneLine::in_bounds(int slot_index) const noexcept
{
    return slot_index >= 0 && slot_index < slot_count();
}

bool RuneLine::set_rune(int slot_index, std::shared_ptr<const Rune> rune)
{
    if (slot_index < 0)
    {
        return false;
    }

    if (slot_index >= slot_count())
    {
        _runes.resize(slot_index + 1, nullptr);
    }

    _runes[slot_index] = std::move(rune);

    return true;
}

std::shared_ptr<const Rune> RuneLine::rune_at(int slot_index) const noexcept
{
    if (!in_bounds(slot_index))
    {
        return nullptr;
    }

    return _runes[slot_index];
}

RuneLoadout RuneLine::evaluate() const
{
    RuneLoadout loadout;
    std::vector<RuneWeaponNode> weapons = evaluate_weapons();

    if (!weapons.empty())
    {
        loadout = weapons.front().loadout;
    }

    return loadout;
}

std::vector<RuneWeaponNode> RuneLine::evaluate_weapons() const
{
    std::vector<RuneWeaponNode> weapons;
    int slot_index = 0;

    while (slot_index < slot_count())
    {
        std::shared_ptr<const Rune> rune = rune_at(slot_index);
        if (!rune)
        {
            ++slot_index;
            continue;
        }

        if (rune->type() != RuneType::Weapon)
        {
            ++slot_index;
            continue;
        }

        const WeaponRune *weapon_rune = dynamic_cast<const WeaponRune *>(rune.get());
        if (!weapon_rune)
        {
            ++slot_index;
            continue;
        }

        const WeaponConsumptionData consumption = weapon_rune->consumption();
        int scope_end_index = slot_index + 1 + consumption.consumed_runes;
        if (scope_end_index > slot_count())
        {
            scope_end_index = slot_count();
        }

        int subtree_end_index = scope_end_index;
        weapons.push_back(build_weapon_node(slot_index, scope_end_index, subtree_end_index));
        slot_index = subtree_end_index;
    }

    return weapons;
}

RuneWeaponNode RuneLine::build_weapon_node(int weapon_slot_index, int scope_end_index, int &subtree_end_index) const
{
    RuneWeaponNode node;
    node.slot_index = weapon_slot_index;

    std::shared_ptr<const Rune> weapon_slot = rune_at(weapon_slot_index);
    const WeaponRune *weapon_rune = dynamic_cast<const WeaponRune *>(weapon_slot.get());
    if (!weapon_rune)
    {
        return node;
    }

    node.consumption = weapon_rune->consumption();
    weapon_rune->apply_weapon(node.loadout);

    // By default the subtree only covers the weapon itself, it will grow
    // to include consumed children as they are discovered.
    subtree_end_index = weapon_slot_index + 1;

    int slot_index = weapon_slot_index + 1;
    while (slot_index < scope_end_index && slot_index < slot_count())
    {
        std::shared_ptr<const Rune> rune = rune_at(slot_index);
        if (!rune)
        {
            ++slot_index;
            continue;
        }

        if (rune->type() == RuneType::Weapon)
        {
            const WeaponRune *child_weapon = dynamic_cast<const WeaponRune *>(rune.get());
            if (child_weapon)
            {
                const WeaponConsumptionData child_consumption = child_weapon->consumption();
                int child_scope_end = slot_index + 1 + child_consumption.consumed_runes;

                if (child_scope_end > scope_end_index)
                {
                    child_scope_end = scope_end_index;
                }

                if (child_scope_end > slot_count())
                {
                    child_scope_end = slot_count();
                }

                int child_subtree_end_index = child_scope_end;
                RuneWeaponNode child_node = build_weapon_node(slot_index, child_scope_end, child_subtree_end_index);
                child_node.consumption = child_consumption;

                if (child_subtree_end_index > subtree_end_index)
                {
                    subtree_end_index = child_subtree_end_index;
                }

                if (node.consumption.consume_type == WeaponConsumeType::AddModifiers)
                {
                    merge_loadout(node.loadout, child_node.loadout);
                    node.children.insert(node.children.end(), child_node.children.begin(), child_node.children.end());
                }
                else
                {
                    node.children.push_back(std::move(child_node));
                }

                slot_index = child_subtree_end_index - 1;
            }
        }
        else if (rune->type() == RuneType::Stat)
        {
            const StatRune *stat_rune = dynamic_cast<const StatRune *>(rune.get());
            if (stat_rune)
            {
                stat_rune->apply_stat(node.loadout);
            }
        }
        else if (rune->type() == RuneType::Behavior)
        {
            const BehaviorRune *behavior_rune = dynamic_cast<const BehaviorRune *>(rune.get());
            if (behavior_rune)
            {
                behavior_rune->apply_weapon(node.loadout);
            }
        }

        ++slot_index;
    }

    return node;
}
