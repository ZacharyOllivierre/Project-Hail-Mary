#pragma once

#include "../enemy.h"

class SkeletonEnemy final : public Enemy
{
public:
    inline static constexpr engine::core::Vector2 DefaultSize{64.0f, 64.0f};

    explicit SkeletonEnemy(const engine::core::Vector2& position)
        : Enemy("enemy/Skeleton", position, DefaultSize)
    {
        set_hp(100.0f);
        set_move_speed(80.0f);
    }
};
