#pragma once

#include "../engine/core/geometry/vector2.h"
#include "projectile.h"

#include <memory>

// Next step is setting up wand to alter bullet attributes via wand cusomization

class Wand
{
public:
    Wand() = default;

    std::unique_ptr<Projectile> attack(
        const Vector2 &origin,
        const Vector2 &direction) const;

private:
    float _bullet_speed = 300.0f;
};