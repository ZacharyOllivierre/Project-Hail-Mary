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
        refresh_collision_shapes();
    }

protected:
    [[nodiscard]] engine::core::Rect make_body_collision_rect(
        const engine::core::Rect& render_rect) const noexcept override
    {
        return make_bottom_centered_scaled_rect(render_rect, {0.82f, 0.26f});
    }

    [[nodiscard]] engine::core::Rect make_hurt_box_rect(
        const engine::core::Rect& render_rect) const noexcept override
    {
        return make_centered_scaled_rect(render_rect, {0.90f, 0.72f});
    }
};
