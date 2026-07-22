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
        refresh_collision_shapes();
    }

protected:
    [[nodiscard]] engine::core::Rect make_body_collision_rect(
        const engine::core::Rect& render_rect) const noexcept override
    {
        return make_bottom_centered_scaled_rect(render_rect, {0.56f, 0.32f});
    }

    [[nodiscard]] engine::core::Rect make_hurt_box_rect(
        const engine::core::Rect& render_rect) const noexcept override
    {
        return make_centered_scaled_rect(render_rect, {0.70f, 0.84f});
    }
};
