#pragma once

#include "../../../engine/core/geometry/vector2.h"

class Bullet;
namespace engine::core
{
    class GameObject;
}

struct BulletBehaviorContext
{
    Bullet &bullet;

    engine::core::GameObject *entity = nullptr;

    engine::core::Vector2 collision_direction = engine::core::Vector2::zero();

    double delta = 0.0;
};