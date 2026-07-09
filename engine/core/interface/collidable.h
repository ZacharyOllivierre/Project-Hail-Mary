#pragma once

#include "../geometry/rect.h"

class Collidable
{
public:
    virtual ~Collidable() = default;

    [[nodiscard]] virtual Rect collision_rect() const noexcept = 0;

    // Added for wand
    virtual void on_collision() noexcept
    {
    }

    [[nodiscard]] virtual bool is_trigger() const noexcept
    {
        return false;
    }

    [[nodiscard]] virtual bool collision_enabled() const noexcept
    {
        return true;
    }
};
