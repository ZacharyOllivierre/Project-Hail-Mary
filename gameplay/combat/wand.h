#pragma once

#include "../../engine/core/geometry/vector2.h"
#include "projectile.h"
#include "bullet.h"

#include <memory>
#include <utility>
#include <cstdlib>

using std::unique_ptr;
using std::vector;

/*
ShotDescriptor is a per shot intention record, not a live projectile. It carries the bullet template,
the relative spawn offset, and the spawn delay. The room scene resolves the final world position when the
shot actually fires so delayed shots still spawn relative to the character’s current position.
*/
struct ShotDescriptor
{
    Bullet_Attributes bullet_attributes;

    // Bullet position position relative to character pos
    engine::core::Vector2 spawn_offset;
    float spawn_delay_sec;
};

enum class SpreadStyle
{
    Uniform,
    Circular,
    Random
};

enum class ShotStyle
{
    Simultaneous,
    Sequential,
    ReverseSequential
};

struct WandAttributes
{
    // Need to implement these two
    float cooldown_seconds;
    float mana_cost;

    int bullet_count = 10;

    SpreadStyle spread_style = SpreadStyle::Circular;
    float spread_degrees = 30.0;

    ShotStyle shot_style = ShotStyle::Sequential;
    float first_shot_delay = 0.0f;
    float shot_delay_sec = 0.09f;

    float spawn_distance = 32.0f;
};

// Implement later
// Pull - pulls enemies towards bullet

class Wand
{
public:
    Wand() = default;

    [[nodiscard]] vector<ShotDescriptor> attack(const engine::core::Vector2 &direction);

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
};
