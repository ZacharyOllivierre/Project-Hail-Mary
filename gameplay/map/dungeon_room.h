#pragma once

#include "../../engine/core/game_object.h"
#include "../../engine/resources/resource_manager.h"

#include "map_generator.h"
#include "tile_map.h"

#include <memory>

class DungeonRoom : public GameObject
{
public:
    DungeonRoom();

    void generate();

    void submit_render_commands(std::vector<RenderCommand> &commands) const override;

private:
    Vector2 _grid_size{24.0f, 16.0f};
    TileMap _tile_map;
    MapConfig _config;
    std::unique_ptr<MapGenerator> _generator;
    SDL_Texture *_tile_sheet_texture = nullptr;
};