#include "enemy_generator.h"

#include "../map/tile_data.h"

#include <utility>

std::vector<std::unique_ptr<Enemy>> EnemyGenerator::generate(
    const DungeonRoom& room,const EnemyGenerationConfig& config)
{
    std::vector<std::unique_ptr<Enemy>> enemies;
    if (config.character_id.empty() || config.count == 0)
        return enemies;

    const TileMap& tile_map = room.tile_map();
    std::vector<engine::core::Vector2> floor_tiles;
    floor_tiles.reserve(static_cast<std::size_t>(tile_map.width() * tile_map.height()));

    for (int y = 0; y < tile_map.height(); ++y)
    {
        for (int x = 0; x < tile_map.width(); ++x)
        {
            const Tile& tile = tile_map.get(x, y);
            if (tile.type == TileType::Floor && !tile.collidable)
                floor_tiles.push_back(tile.position);
        }
    }

    if (floor_tiles.empty())
        return enemies;

    enemies.reserve(config.count);

    const engine::core::Vector2 tile_size = room.tile_render_size();
    const engine::core::Vector2 centering_offset = (tile_size - config.size) * 0.5f;

    for (std::size_t i = 0; i < config.count; ++i)
    {
        const engine::core::Vector2& tile = _random.pick(floor_tiles);
        const engine::core::Vector2 position{
            room.position().x + tile.x * tile_size.x + centering_offset.x,
            room.position().y + tile.y * tile_size.y + centering_offset.y};

        auto enemy = std::make_unique<Enemy>(config.character_id,
            position,config.size);

        enemy->set_move_speed(config.move_speed);
        enemies.push_back(std::move(enemy));
    }

    return enemies;
}
