#pragma once

#include "tile_data.h"
#include "tile_map.h"

int random_value(int min_value, int max_value);

#include "../../engine/core/geometry/rect.h"

enum class Side
{
    Top,
    Bottom,
    Left,
    Right,
    None
};

class MapGenerator
{
public:
    MapGenerator(const engine::core::Vector2 gridDimensions, MapConfig config, TileMap &grid);

    void generateRoom();
    void clearGrid();

private:
    void initGrid();

    engine::core::Rect buildBaseRoom();
    engine::core::Rect buildSubRoom(engine::core::Rect &base, Side side = Side::None);

    void addRecToGrid(engine::core::Rect &rec);
    void classifyRecs();

    bool isSolid(const int x, const int y);

    void resetTile(Tile &tile);

private:
    engine::core::Vector2 _gridDimensions;
    TileMap &grid;

    MapConfig config;
};
