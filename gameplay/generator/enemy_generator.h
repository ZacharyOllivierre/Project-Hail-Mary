#pragma once

#include "../../engine/core/geometry/vector2.h"
#include "../../engine/tools/random_generator.h"
#include "../characters/enemy.h"
#include "../map/dungeon_room.h"

#include <cstddef>
#include <memory>
#include <vector>

enum class EnemyType
{
    Skeleton,
    SkeletonElite,
    Slime,
    GoblinWitch,
    Wizard
};

struct EnemyGenerationConfig
{
    EnemyType type = EnemyType::Skeleton;
    std::size_t count = 0;
};

class EnemyGenerator
{
public:
    [[nodiscard]] std::vector<std::unique_ptr<Enemy>> generate(
        const DungeonRoom& room,const EnemyGenerationConfig& config);

private:
    [[nodiscard]] static std::unique_ptr<Enemy> create_enemy(
        EnemyType type,
        const engine::core::Vector2& position);
    [[nodiscard]] static engine::core::Vector2 enemy_size(EnemyType type) noexcept;
    [[nodiscard]] static bool is_spawn_area_available(
        const DungeonRoom& room,
        const engine::core::Vector2& position,
        const engine::core::Vector2& size) noexcept;

    engine::tools::RandomGenerator _random;
};
