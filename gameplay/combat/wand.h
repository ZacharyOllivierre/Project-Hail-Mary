#pragma once

#include "wand_types.h"
#include "wand_debug_data.h"

#include <cstdlib>
#include <vector>

using std::vector;

// Implement later
// Pull - pulls enemies towards bullet

class Wand
{
public:
    Wand();

    [[nodiscard]] vector<ShotDescriptor> attack(const engine::core::Vector2 &direction);

    [[nodiscard]] WandDebugData &debug_data() noexcept;

private:
    [[nodiscard]] ShotDescriptor make_shot(const engine::core::Vector2 &direction, int index);

    float calculate_bullet_angle(int index);
    float calc_uniform_spread_angle(int num);
    float calc_circular_spread_angle(int num);
    float calc_random_spread_angle();

    float get_shot_delay(int index);

private:
    WandAttributes _wand_attributes;
    Bullet_Attributes _bullet_attributes;

    WandDebugData _debug_data;
};
