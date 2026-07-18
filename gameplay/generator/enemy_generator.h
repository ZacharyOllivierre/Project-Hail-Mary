#pragma once

#include "../../engine/core/geometry/vector2.h"
#include "../../engine/tools/random_generator.h"
#include "../characters/enemy.h"
#include "../map/dungeon_room.h"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

struct EnemyGenerationConfig
{
    std::string character_id;
    std::size_t count = 0;
    engine::core::Vector2 size = engine::core::Vector2::zero();
    float move_speed = 0.0f;
};

class EnemyGenerator
{
public:
    [[nodiscard]] std::vector<std::unique_ptr<Enemy>> generate(
        const DungeonRoom& room,const EnemyGenerationConfig& config);

private:
    engine::tools::RandomGenerator _random;
};
