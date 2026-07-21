#pragma once

#include "../enemy.h"

class GoblinWitchEnemy final : public Enemy
{
public:
    inline static constexpr engine::core::Vector2 DefaultSize{56.0f, 56.0f};

    explicit GoblinWitchEnemy(const engine::core::Vector2& position)
        : Enemy("enemy/GoblinWitch", position, DefaultSize)
    {
        set_hp(80.0f);
        set_move_speed(70.0f);
    }
};
