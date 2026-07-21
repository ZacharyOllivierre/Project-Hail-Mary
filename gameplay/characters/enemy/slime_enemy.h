#pragma once

#include "../enemy.h"

class SlimeEnemy final : public Enemy
{
public:
    inline static constexpr engine::core::Vector2 DefaultSize{40.0f, 40.0f};

    explicit SlimeEnemy(const engine::core::Vector2& position)
        : Enemy("enemy/Slime", position, DefaultSize)
    {
        set_hp(60.0f);
        set_move_speed(50.0f);
    }
};
