#pragma once

#include "../../engine/core/geometry/vector2.h"

// Base config dont delete
// struct MapConfig
// {
//     // Can't go lower than 6 for some reason
//     Vector2 baseRoomSize = {6, 6};

//     int numSubRooms = 7;
//     Vector2 minSubRoomSize = {3, 3};

//     // No lower than 2
//     int minGap = 2;
// };

// Testing config
// TODO map config can crash application if vals are larger than tilemap size
struct MapConfig
{
    Vector2 baseRoomSize = {5, 5};
    int numSubRooms = 7;
    Vector2 minSubRoomSize = {4, 3};
    int minGap = 2;
};