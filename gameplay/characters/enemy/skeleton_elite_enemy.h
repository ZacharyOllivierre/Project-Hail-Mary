#pragma once

#include "../enemy.h"

class SkeletonEliteEnemy final : public Enemy
{
public:
    inline static constexpr engine::core::Vector2 DefaultSize{96.0f, 96.0f};

    explicit SkeletonEliteEnemy(const engine::core::Vector2& position)
        : Enemy("enemy/SkeletonElite", position, DefaultSize)
    {
        set_hp(200.0f);
        set_move_speed(60.0f);
    }
};
