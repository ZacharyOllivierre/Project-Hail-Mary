#pragma once

#include "../../engine/physics/tile_collision_world.h"
#include "../map/dungeon_room.h"

#include <cmath>

class RoomTileCollisionWorld final : public engine::physics::TileCollisionWorld
{
public:
    void set_room(const DungeonRoom *room) noexcept
    {
        _room = room;
    }

    [[nodiscard]] engine::core::Vector2 world_origin() const noexcept override
    {
        return _room ? _room->position() : engine::core::Vector2::zero();
    }

    [[nodiscard]] engine::core::Vector2 tile_size() const noexcept override
    {
        return _room ? _room->tile_render_size() : engine::core::Vector2::zero();
    }

    [[nodiscard]] int tile_columns() const noexcept override
    {
        return _room ? _room->tile_map().width() : 0;
    }

    [[nodiscard]] int tile_rows() const noexcept override
    {
        return _room ? _room->tile_map().height() : 0;
    }

    [[nodiscard]] bool is_tile_collidable(int x, int y) const noexcept override
    {
        if (!_room)
            return false;

        const TileMap &tile_map = _room->tile_map();
        if (x < 0 || x >= tile_map.width())
            return false;

        if (y < 0 || y >= tile_map.height())
            return false;

        return tile_map.get(x, y).collidable;
    }

    // need because teleport needs to check its complete landing footprint
    [[nodiscard]] bool is_floor_area(const engine::core::Rect &area) const noexcept
    {
        if (!_room || area.is_empty())
            return false;

        const engine::core::Vector2 origin = world_origin();
        const engine::core::Vector2 size = tile_size();
        const int min_x = static_cast<int>(std::floor((area.left() - origin.x) / size.x));
        const int max_x = static_cast<int>(std::floor(
            (area.right() - origin.x - engine::core::Rect::k_epsilon) / size.x));
        const int min_y = static_cast<int>(std::floor((area.top() - origin.y) / size.y));
        const int max_y = static_cast<int>(std::floor(
            (area.bottom() - origin.y - engine::core::Rect::k_epsilon) / size.y));

        for (int y = min_y; y <= max_y; ++y)
        {
            for (int x = min_x; x <= max_x; ++x)
            {
                if (x < 0 || x >= tile_columns() || y < 0 || y >= tile_rows())
                    return false;

                const Tile &tile = _room->tile_map().get(x, y);
                if (tile.type != TileType::Floor || tile.collidable)
                    return false;
            }
        }
        return true;
    }

private:
    const DungeonRoom *_room = nullptr;
};
