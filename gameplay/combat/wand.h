#pragma once

#include "wand_types.h"
#include "wand_debug_data.h"
#include "grid/rune_line.h"

#include <cstdlib>
#include <vector>

using std::vector;

class Wand
{
public:
    Wand();

    [[nodiscard]] vector<ShotDescriptor> attack(const engine::core::Vector2 &direction);

    [[nodiscard]] WandDebugData &debug_data() noexcept;

private:
    [[nodiscard]] ShotDescriptor make_shot(const RuneLoadout &loadout, const engine::core::Vector2 &direction,
                                           int index, float delay_offset);

    int append_weapon_shots(const RuneWeaponNode &weapon_node, const engine::core::Vector2 &direction,
                            float delay_offset, std::vector<ShotDescriptor> &out_shots);

    std::shared_ptr<StatusEffect> roll_for_effect(const WandAttributes &wand_attributes);

    float calculate_bullet_angle(const WandAttributes &wand_attributes, int index);
    float calc_uniform_spread_angle(const WandAttributes &wand_attributes, int num);
    float calc_circular_spread_angle(const WandAttributes &wand_attributes, int num);
    float calc_random_spread_angle(const WandAttributes &wand_attributes);

    float get_shot_delay(const WandAttributes &wand_attributes, int index);

    void seed_test_runes();

private:
    WandAttributes _wand_attributes;
    Bullet_Attributes _bullet_attributes;
    RuneLine _rune_line;

    WandDebugData _debug_data;
};
