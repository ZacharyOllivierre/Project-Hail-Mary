#pragma once

#include "../../engine/core/geometry/vector2.h"

// Base config dont delete
struct MapConfig
{
    // Can't go lower than 6 for some reason
    engine::core::Vector2 baseRoomSize = {10, 10};

    int numSubRooms = 7;
    engine::core::Vector2 minSubRoomSize = {3, 3};

    // No lower than 2
    int minGap = 2;
};
