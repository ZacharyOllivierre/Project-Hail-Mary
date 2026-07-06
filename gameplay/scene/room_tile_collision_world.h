#pragma once

#include "../../engine/core/tile_collision_world.h"
#include "../map/dungeon_room.h"

class RoomTileCollisionWorld final : public TileCollisionWorld
{
public:
    void set_room(const DungeonRoom* room) noexcept
    {
        _room = room;
    }

    [[nodiscard]] Vector2 world_origin() const noexcept override
    {
        return _room ? _room->position() : Vector2::zero();
    }

    [[nodiscard]] Vector2 tile_size() const noexcept override
    {
        return _room ? _room->tile_render_size() : Vector2::zero();
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

        const TileMap& tile_map = _room->tile_map();
        if (x < 0 || x >= tile_map.width())
            return false;

        if (y < 0 || y >= tile_map.height())
            return false;

        return tile_map.get(x, y).collidable;
    }

private:
    const DungeonRoom* _room = nullptr;
};
