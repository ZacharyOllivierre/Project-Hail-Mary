#pragma once

#include "tile_data.h"
#include <vector>

using std::vector;

class TileMap
{
public:
    void resize(int width, int height);

    Tile &get(int x, int y);
    const Tile &get(int x, int y) const;
    void set(int x, int y, const Tile &value);

    int width() const;
    int height() const;

    vector<vector<Tile>> &data();
    const vector<vector<Tile>> &data() const;

private:
    vector<vector<Tile>> tiles;
};
