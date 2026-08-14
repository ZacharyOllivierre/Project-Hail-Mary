#include "game_object_service.h"

#include "../characters/character.h"
#include "../characters/enemy.h"

Character* GameObjectService::find_nearest_character(const engine::core::Vector2& origin,
    const engine::core::GameObject* exclude) const
{
    return find_nearest_object_if<Character>(origin,exclude,nullptr,
        [](const Character& character)
        {
            return !character.is_dead();
        });
}

Enemy* GameObjectService::find_nearest_enemy(const engine::core::Vector2& origin) const
{
    return find_nearest_object_if<Enemy>(origin,nullptr,nullptr,
        [](const Enemy& enemy)
        {
            return !enemy.is_dead();
        });
}
