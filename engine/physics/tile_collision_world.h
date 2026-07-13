#pragma once

#include "../core/geometry/vector2.h"

namespace engine::physics
{

class TileCollisionWorld
{
public:
    virtual ~TileCollisionWorld() = default;

    [[nodiscard]] virtual engine::core::Vector2 world_origin() const noexcept = 0;
    [[nodiscard]] virtual engine::core::Vector2 tile_size() const noexcept = 0;
    [[nodiscard]] virtual int tile_columns() const noexcept = 0;
    [[nodiscard]] virtual int tile_rows() const noexcept = 0;
    [[nodiscard]] virtual bool is_tile_collidable(int x, int y) const noexcept = 0;
};
}
