#pragma once

#include "../enemy.h"

class WizardEnemy final : public Enemy
{
public:
    inline static constexpr engine::core::Vector2 DefaultSize{72.0f, 72.0f};

    explicit WizardEnemy(const engine::core::Vector2& position)
        : Enemy("enemy/Wizard", position, DefaultSize)
    {
        set_hp(100.0f);
        set_move_speed(60.0f);
    }
};
