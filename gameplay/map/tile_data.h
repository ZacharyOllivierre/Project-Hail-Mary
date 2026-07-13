#pragma once

#include "../../engine/core/geometry/vector2.h"
#include "map_config.h"

enum class TileType
{
    Void,
    Floor,

    TopWall,
    BottomWall,
    LeftWall,
    RightWall,

    TopLeftCorner,
    TopRightCorner,
    BottomLeftCorner,
    BottomRightCorner,

    InnerTopLeft,
    InnerTopRight,
    InnerBottomLeft,
    InnerBottomRight,

    Count
};

struct Tile
{
    // generation data
    ::engine::core::Vector2 position = {0, 0};
    bool solid = false;
    TileType type = TileType::Void;

    // physics data
    bool collidable = false;
};
