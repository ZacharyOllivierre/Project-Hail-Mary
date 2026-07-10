#pragma once

#include "../engine/core/geometry/vector2.h"
#include "projectile.h"
#include "bullet.h"

#include <memory>
#include <utility>

// Should probably be in character - not going to implement it yet
struct Wand_Attributes
{
    float mana_cost = 10.0f;
    float attack_interval = 1.0f;
    // More like mana recharge rate, recharge amount..
    // Will need to be exposed to wand if wand "peices are going to affect it"
};

// Attributes
// Bouncing
// Multiply - Multiple projectiles for one attack

// Implement later
// Homing - pulls towards nearest enemy
// Pull - pulls enemies towards bullet
// Divide - Sets minimum multiply to 2 // Spread shots by 2, 4, 8

class Wand
{
public:
    Wand() = default;

    std::unique_ptr<Projectile> attack(const Vector2 &origin, const Vector2 &direction);

private:
    Bullet_Attributes bullet_attributes;
};