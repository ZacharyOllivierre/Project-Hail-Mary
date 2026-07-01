#include "map_generator.h"

#include <cstdlib>
#include <ctime>
#include <utility>

int random_value(int min_value, int max_value)
{
    if (max_value < min_value)
        std::swap(min_value, max_value);

    return min_value + (std::rand() % (max_value - min_value + 1));
}

MapGenerator::MapGenerator(const Vector2 gridDimensions, MapConfig config, TileMap &grid)
    : _gridDimensions(gridDimensions), grid(grid), config(config)
{
    static bool seeded = false;
    if (!seeded)
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }

    initGrid();
}

void MapGenerator::generateRoom()
{
    clearGrid();

    Rect base = buildBaseRoom();
    addRecToGrid(base);

    for (int i = 0; i < config.numSubRooms; i++)
    {
        Rect subRoom = buildSubRoom(base);
        addRecToGrid(subRoom);
    }

    classifyRecs();
}

void MapGenerator::clearGrid()
{
    auto &tiles = grid.data();

    for (int y = 0; y < (int)tiles.size(); y++)
    {
        for (int x = 0; x < (int)tiles[y].size(); x++)
        {
            resetTile(tiles[y][x]);
        }
    }
}

void MapGenerator::initGrid()
{
    grid.resize(static_cast<int>(_gridDimensions.x), static_cast<int>(_gridDimensions.y));

    auto &tiles = grid.data();

    for (int y = 0; y < (int)tiles.size(); y++)
    {
        for (int x = 0; x < (int)tiles[y].size(); x++)
        {
            tiles[y][x].position = {static_cast<float>(x), static_cast<float>(y)};
            resetTile(tiles[y][x]);
        }
    }
}

Rect MapGenerator::buildBaseRoom()
{
    Rect room{0.0f, 0.0f, config.baseRoomSize.x, config.baseRoomSize.y};
    Vector2 center = {_gridDimensions.x / 2.0f, _gridDimensions.y / 2.0f};

    room.set_x(center.x - (room.width() / 2.0f));
    room.set_y(center.y - (room.height() / 2.0f));

    return room;
}

// Builds room adjacent to main room, bounded by grid size / minimum gap
Rect MapGenerator::buildSubRoom(Rect &base, Side side)
{
    if (side == Side::None)
    {
        side = static_cast<Side>(random_value(static_cast<int>(Side::Top), static_cast<int>(Side::Right)));
    }

    Rect room;

    int baseRight = static_cast<int>(base.x() + base.width() - 1.0f);
    int baseBottom = static_cast<int>(base.y() + base.height() - 1.0f);

    Vector2 minSize = config.minSubRoomSize;
    Vector2 anchorPoint;

    /*When taking only outer most edges of full shape minGap space
    will always be walkable between rooms*/
    int minGap = config.minGap;

    bool generateForward = random_value(0, 1) == 1;

    if (side == Side::Top || side == Side::Bottom)
    {
        if (side == Side::Top)
        {
            anchorPoint.y = base.y();
            anchorPoint.x = static_cast<float>(random_value(static_cast<int>(base.x() + minGap + 1), baseRight - (minGap + 1)));

            room.set_height(static_cast<float>(random_value(static_cast<int>(minSize.y), static_cast<int>(anchorPoint.y))));
            room.set_y(anchorPoint.y - room.height() + 1.0f);
        }
        else
        {
            anchorPoint.y = static_cast<float>(baseBottom);
            anchorPoint.x = static_cast<float>(random_value(static_cast<int>(base.x() + minGap + 1), baseRight - (minGap + 1)));

            room.set_height(static_cast<float>(random_value(static_cast<int>(minSize.y), static_cast<int>(_gridDimensions.y - anchorPoint.y))));
            room.set_y(anchorPoint.y);
        }

        if (generateForward)
        {
            room.set_width(static_cast<float>(random_value(static_cast<int>(minSize.x), static_cast<int>(_gridDimensions.x - anchorPoint.x))));
            room.set_x(anchorPoint.x);
        }
        else
        {
            room.set_width(static_cast<float>(random_value(static_cast<int>(minSize.x), static_cast<int>(anchorPoint.x))));
            room.set_x(anchorPoint.x - room.width() + 1.0f);
        }
    }
    else
    {
        if (side == Side::Left)
        {
            anchorPoint.x = base.x();
            anchorPoint.y = static_cast<float>(random_value(static_cast<int>(base.y() + minGap + 1), baseBottom - (minGap + 1)));

            room.set_width(static_cast<float>(random_value(static_cast<int>(minSize.x), static_cast<int>(anchorPoint.x))));
            room.set_x(anchorPoint.x - room.width() + 1.0f);
        }
        else
        {
            anchorPoint.x = static_cast<float>(baseRight);
            anchorPoint.y = static_cast<float>(random_value(static_cast<int>(base.y() + minGap + 1), baseBottom - (minGap + 1)));

            room.set_width(static_cast<float>(random_value(static_cast<int>(minSize.x), static_cast<int>(_gridDimensions.x - anchorPoint.x))));
            room.set_x(anchorPoint.x);
        }

        if (generateForward)
        {
            room.set_height(static_cast<float>(random_value(static_cast<int>(minSize.y), static_cast<int>(_gridDimensions.y - anchorPoint.y))));
            room.set_y(anchorPoint.y);
        }
        else
        {
            room.set_height(static_cast<float>(random_value(static_cast<int>(minSize.y), static_cast<int>(anchorPoint.y))));
            room.set_y(anchorPoint.y - room.height() + 1.0f);
        }
    }

    return room;
}

void MapGenerator::addRecToGrid(Rect &rec)
{
    auto &tiles = grid.data();

    for (int y = static_cast<int>(rec.y()); y < static_cast<int>(rec.y() + rec.height()); y++)
    {
        for (int x = static_cast<int>(rec.x()); x < static_cast<int>(rec.x() + rec.width()); x++)
        {
            tiles[y][x].solid = true;
            tiles[y][x].type = TileType::Floor;
        }
    }
}

void MapGenerator::classifyRecs()
{
    auto &tiles = grid.data();

    for (int y = 0; y < (int)tiles.size(); y++)
    {
        for (int x = 0; x < (int)tiles[y].size(); x++)
        {
            Tile *tile = &tiles[y][x];

            if (tile->solid == false)
            {
                continue;
            }

            bool N = isSolid(x, y - 1);
            bool S = isSolid(x, y + 1);
            bool E = isSolid(x + 1, y);
            bool W = isSolid(x - 1, y);

            bool NE = isSolid(x + 1, y - 1);
            bool NW = isSolid(x - 1, y - 1);
            bool SE = isSolid(x + 1, y + 1);
            bool SW = isSolid(x - 1, y + 1);

            if (!N && !W)
                tile->type = TileType::TopLeftCorner;
            else if (!N && !E)
                tile->type = TileType::TopRightCorner;
            else if (!S && !W)
                tile->type = TileType::BottomLeftCorner;
            else if (!S && !E)
                tile->type = TileType::BottomRightCorner;
            else if (N && W && !NW)
                tile->type = TileType::InnerTopLeft;
            else if (N && E && !NE)
                tile->type = TileType::InnerTopRight;
            else if (S && W && !SW)
                tile->type = TileType::InnerBottomLeft;
            else if (S && E && !SE)
                tile->type = TileType::InnerBottomRight;
            else if (!N)
                tile->type = TileType::TopWall;
            else if (!S)
                tile->type = TileType::BottomWall;
            else if (!W)
                tile->type = TileType::LeftWall;
            else if (!E)
                tile->type = TileType::RightWall;
            else
                tile->type = TileType::Floor;
        }
    }
}

bool MapGenerator::isSolid(const int x, const int y)
{
    if (x < 0 || x >= static_cast<int>(_gridDimensions.x))
        return false;

    if (y < 0 || y >= static_cast<int>(_gridDimensions.y))
        return false;

    return grid.data()[y][x].solid;
}

void MapGenerator::resetTile(Tile &tile)
{
    tile.solid = false;
    tile.type = TileType::Void;
    tile.tileId = 0;
}
