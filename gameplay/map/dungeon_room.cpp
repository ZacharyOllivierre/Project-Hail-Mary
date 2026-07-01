#include "dungeon_room.h"

#include "../../engine/core/render/render_command.h"
#include "../../engine/resources/resource_manager.h"

#include <SDL.h>

#include <utility>

#include <iostream>

namespace
{
    constexpr int kTileSpriteSize = 32;
    constexpr float kTileRenderSize = 64.0f;
    constexpr float kRoomOriginX = 0.0f;
    constexpr float kRoomOriginY = 0.0f;

    int sprite_index_for_tile(TileType type)
    {
        switch (type)
        {
        case TileType::Void:
            return 13;
        case TileType::Floor:
            return 12;
        case TileType::TopWall:
            return 0;
        case TileType::BottomWall:
            return 1;
        case TileType::LeftWall:
            return 2;
        case TileType::RightWall:
            return 3;
        case TileType::TopLeftCorner:
            return 5;
        case TileType::TopRightCorner:
            return 4;
        case TileType::BottomLeftCorner:
            return 6;
        case TileType::BottomRightCorner:
            return 7;
        case TileType::InnerTopLeft:
            return 10;
        case TileType::InnerTopRight:
            return 11;
        case TileType::InnerBottomLeft:
            return 8;
        case TileType::InnerBottomRight:
            return 9;
        default:
            return 13;
        }
    }
}

DungeonRoom::DungeonRoom()
    : GameObject(DepthLayer::Terrain),
      _grid_size{25.0f, 25.0f},
      _generator(std::make_unique<MapGenerator>(_grid_size, _config, _tile_map))
{
    set_position({kRoomOriginX, kRoomOriginY});
    set_size({_grid_size.x * kTileRenderSize, _grid_size.y * kTileRenderSize});
    if (ResourceManager::instance())
    {
        _tile_sheet_texture = ResourceManager::instance()->find_texture("room_tiles");
    }

    if (!_tile_sheet_texture)
    {
        std::cout << "DungeonRoom texture not found in resource manager." << std::endl;
    }

    generate();
}

void DungeonRoom::generate()
{
    _tile_map.resize(static_cast<int>(_grid_size.x), static_cast<int>(_grid_size.y));

    if (!_generator)
    {
        _generator = std::make_unique<MapGenerator>(_grid_size, _config, _tile_map);
    }

    _generator->generateRoom();
}

void DungeonRoom::submit_render_commands(std::vector<RenderCommand> &commands) const
{
    if (!_tile_sheet_texture)
        return;

    const auto &tiles = _tile_map.data();

    for (int y = 0; y < _tile_map.height(); ++y)
    {
        for (int x = 0; x < _tile_map.width(); ++x)
        {
            const Tile &tile = tiles[y][x];
            if (!tile.solid)
                continue;

            RenderCommand command;
            command.texture = _tile_sheet_texture;
            command.command_rect = Rect{
                position().x + static_cast<float>(x) * kTileRenderSize,
                position().y + static_cast<float>(y) * kTileRenderSize,
                kTileRenderSize,
                kTileRenderSize};
            command.use_src_rect = true;
            command.src_rect = Rect{
                static_cast<float>(sprite_index_for_tile(tile.type) * kTileSpriteSize),
                0.0f,
                static_cast<float>(kTileSpriteSize),
                static_cast<float>(kTileSpriteSize)};
            commands.push_back(std::move(command));
        }
    }
}
