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
        refresh_collision_shapes();
    }

protected:
    [[nodiscard]] engine::core::Rect make_body_collision_rect(
        const engine::core::Rect& render_rect) const noexcept override
    {
        return make_bottom_centered_scaled_rect(render_rect, {0.58f, 0.34f});
    }

    [[nodiscard]] engine::core::Rect make_hurt_box_rect(
        const engine::core::Rect& render_rect) const noexcept override
    {
        return make_centered_scaled_rect(render_rect, {0.72f, 0.86f});
    }
};
