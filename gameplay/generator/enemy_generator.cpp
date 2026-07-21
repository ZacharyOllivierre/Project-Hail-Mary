#include "enemy_generator.h"

#include "../characters/enemy/goblin_witch_enemy.h"
#include "../characters/enemy/skeleton_elite_enemy.h"
#include "../characters/enemy/skeleton_enemy.h"
#include "../characters/enemy/slime_enemy.h"
#include "../characters/enemy/wizard_enemy.h"
#include "../map/tile_data.h"

#include <cmath>
#include <utility>

std::vector<std::unique_ptr<Enemy>> EnemyGenerator::generate(
    const DungeonRoom& room,const EnemyGenerationConfig& config)
{
    std::vector<std::unique_ptr<Enemy>> enemies;
    if (config.count == 0)
        return enemies;

    const TileMap& tile_map = room.tile_map();
    const engine::core::Vector2 tile_size = room.tile_render_size();
    const engine::core::Vector2 size = enemy_size(config.type);
    const engine::core::Vector2 centering_offset = (tile_size - size) * 0.5f;

    std::vector<engine::core::Vector2> spawn_positions;
    spawn_positions.reserve(static_cast<std::size_t>(tile_map.width() * tile_map.height()));

    for (int y = 0; y < tile_map.height(); ++y)
    {
        for (int x = 0; x < tile_map.width(); ++x)
        {
            const Tile& tile = tile_map.get(x, y);
            if (tile.type != TileType::Floor || tile.collidable)
                continue;

            const engine::core::Vector2 position{
                room.position().x + tile.position.x * tile_size.x + centering_offset.x,
                room.position().y + tile.position.y * tile_size.y + centering_offset.y};

            if (is_spawn_area_available(room, position, size))
                spawn_positions.push_back(position);
        }
    }

    if (spawn_positions.empty())
        return enemies;

    enemies.reserve(config.count);

    for (std::size_t i = 0; i < config.count; ++i)
    {
        const engine::core::Vector2& position = _random.pick(spawn_positions);
        if (std::unique_ptr<Enemy> enemy = create_enemy(config.type, position))
            enemies.push_back(std::move(enemy));
    }

    return enemies;
}

std::unique_ptr<Enemy> EnemyGenerator::create_enemy(
    EnemyType type,
    const engine::core::Vector2& position)
{
    switch (type)
    {
    case EnemyType::Skeleton:
        return std::make_unique<SkeletonEnemy>(position);
    case EnemyType::SkeletonElite:
        return std::make_unique<SkeletonEliteEnemy>(position);
    case EnemyType::Slime:
        return std::make_unique<SlimeEnemy>(position);
    case EnemyType::GoblinWitch:
        return std::make_unique<GoblinWitchEnemy>(position);
    case EnemyType::Wizard:
        return std::make_unique<WizardEnemy>(position);
    }

    return nullptr;
}

engine::core::Vector2 EnemyGenerator::enemy_size(EnemyType type) noexcept
{
    switch (type)
    {
    case EnemyType::Skeleton:
        return SkeletonEnemy::DefaultSize;
    case EnemyType::SkeletonElite:
        return SkeletonEliteEnemy::DefaultSize;
    case EnemyType::Slime:
        return SlimeEnemy::DefaultSize;
    case EnemyType::GoblinWitch:
        return GoblinWitchEnemy::DefaultSize;
    case EnemyType::Wizard:
        return WizardEnemy::DefaultSize;
    }

    return engine::core::Vector2::zero();
}

bool EnemyGenerator::is_spawn_area_available(
    const DungeonRoom& room,
    const engine::core::Vector2& position,
    const engine::core::Vector2& size) noexcept
{
    const engine::core::Vector2 tile_size = room.tile_render_size();
    if (tile_size.x <= engine::core::Vector2::k_epsilon ||
        tile_size.y <= engine::core::Vector2::k_epsilon ||
        size.x <= engine::core::Vector2::k_epsilon ||
        size.y <= engine::core::Vector2::k_epsilon)
    {
        return false;
    }

    const engine::core::Vector2 origin = room.position();
    const float right = position.x + size.x - engine::core::Vector2::k_epsilon;
    const float bottom = position.y + size.y - engine::core::Vector2::k_epsilon;

    const int min_x = static_cast<int>(std::floor((position.x - origin.x) / tile_size.x));
    const int max_x = static_cast<int>(std::floor((right - origin.x) / tile_size.x));
    const int min_y = static_cast<int>(std::floor((position.y - origin.y) / tile_size.y));
    const int max_y = static_cast<int>(std::floor((bottom - origin.y) / tile_size.y));

    const TileMap& tile_map = room.tile_map();
    if (min_x < 0 || min_y < 0 || max_x >= tile_map.width() || max_y >= tile_map.height())
        return false;

    for (int y = min_y; y <= max_y; ++y)
    {
        for (int x = min_x; x <= max_x; ++x)
        {
            const Tile& tile = tile_map.get(x, y);
            if (tile.type != TileType::Floor || tile.collidable)
                return false;
        }
    }

    return true;
}
