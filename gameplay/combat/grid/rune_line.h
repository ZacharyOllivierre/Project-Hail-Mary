#pragma once

#include "rune.h"

#include <memory>
#include <vector>

/*
The rune line is a linear list of slots. On fire each weapon consumes a set window of
slots ahead of it, and any stat/behavior runes within that window mutate the weapon's loadout.
Weapons within the consumption window are also applied by their WeaponConsumeType

Fire Nested
First the nested weapon's shot at all sites of the base weapon
ex if base weapon and nested weapon shoot 2 bullets the total shot count will be 6

Add Modifiers
Merges the nested and base weapons stats and behaviors, no extra shots are made
*/

class RuneLine
{
public:
    explicit RuneLine(int starting_slot_count = 1);

    [[nodiscard]] int slot_count() const noexcept;
    [[nodiscard]] bool in_bounds(int slot_index) const noexcept;

    [[nodiscard]] bool set_rune(int slot_index, std::shared_ptr<const Rune> rune);
    [[nodiscard]] std::shared_ptr<const Rune> rune_at(int slot_index) const noexcept;

    [[nodiscard]] RuneLoadout evaluate() const;
    [[nodiscard]] std::vector<RuneWeaponNode> evaluate_weapons() const;

private:
    [[nodiscard]] RuneWeaponNode build_weapon_node(
        int weapon_slot_index,
        int scope_end_index,
        int &subtree_end_index) const;

private:
    std::vector<std::shared_ptr<const Rune>> _runes;
};