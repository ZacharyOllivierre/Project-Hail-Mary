#include "tile_map.h"

void TileMap::resize(int width, int height)
{
    tiles.resize(height);

    for (int i = 0; i < (int)tiles.size(); i++)
    {
        tiles[i].resize(width);
    }
}

Tile &TileMap::get(int x, int y)
{
    return tiles[y][x];
}

const Tile &TileMap::get(int x, int y) const
{
    return tiles[y][x];
}

void TileMap::set(int x, int y, const Tile &value)
{
    tiles[y][x] = value;
}

int TileMap::width() const
{
    if (tiles.empty())
    {
        return 0;
    }
    return (int)tiles[0].size();
}

int TileMap::height() const
{
    if (tiles.empty())
    {
        return 0;
    }
    return (int)tiles.size();
}

vector<vector<Tile>> &TileMap::data()
{
    return tiles;
}

const vector<vector<Tile>> &TileMap::data() const
{
    return tiles;
}
