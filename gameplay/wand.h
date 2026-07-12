#pragma once

#include "../engine/core/geometry/vector2.h"
#include "projectile.h"
#include "bullet.h"

#include <memory>
#include <utility>
#include <cstdlib>

using std::unique_ptr;
using std::vector;

enum class SpreadStyle
{
    Uniform,
    Circular,
    Random
};

struct WandAttributes
{
    // Number of bullets in one attack
    int bullet_count = 4;

    SpreadStyle spread_style = SpreadStyle::Circular;
    float spread_degrees = 80.0;
};

// Implement later
// Homing - pulls towards nearest enemy
// Pull - pulls enemies towards bullet

class Wand
{
public:
    Wand() = default;

    vector<unique_ptr<Projectile>> attack(const Vector2 &origin, const Vector2 &direction);

private:
    void make_bullets(vector<unique_ptr<Projectile>> &projectiles, const Vector2 &direction);

    float calculate_bullet_angle(int index);
    float calc_uniform_spread_angle(int num);
    float calc_circular_spread_angle(int num);
    float calc_random_spread_angle(int num);

private:
    WandAttributes _wand_attributes;
    Bullet_Attributes _bullet_attributes;

    Vector2 _origin;
};