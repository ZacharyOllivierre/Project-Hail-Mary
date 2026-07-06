#pragma once

#include "../geometry/vector2.h"

class KinematicBody
{
public:
    virtual ~KinematicBody() = default;

    [[nodiscard]] virtual Vector2 desired_velocity() const noexcept = 0;
    virtual void apply_translation(const Vector2& delta) noexcept = 0;
};
