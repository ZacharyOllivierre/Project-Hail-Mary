#pragma once

#include "wand_types.h"

#include "../../engine/core/geometry/vector2.h"

#include <vector>

class WandDebugData
{
public:
    WandDebugData(WandAttributes &wand_attributes, Bullet_Attributes &bullet_attributes) noexcept;

    void render_debugger();

private:
    WandAttributes *_wand_attributes = nullptr;
    Bullet_Attributes *_bullet_attributes = nullptr;
};