#pragma once

#include "../../engine/core/game_object.h"
#include "../../engine/core/geometry/vector2.h"
#include "../../engine/scene/scene_manager.h"
#include "../../engine/tools/singleton.h"

#include <concepts>
#include <limits>
#include <optional>

#define GAME_OBJECT_SERVICE (GameObjectService::instance())

class Character;
class Enemy;


class GameObjectService final : public engine::tools::Singleton<GameObjectService>
{
    friend engine::tools::Singleton<GameObjectService>;

public:
    template <typename T = engine::core::GameObject>
        requires std::derived_from<T, engine::core::GameObject>

    [[nodiscard]] T* find_nearest_object(const engine::core::Vector2& origin,
        const engine::core::GameObject* exclude = nullptr) const
    {
        return find_nearest_object_if<T>(
            origin,exclude, std::nullopt,[](const T&) { return true; });
    }

    template <typename T = engine::core::GameObject>
        requires std::derived_from<T, engine::core::GameObject>
    [[nodiscard]] T* find_nearest_object(const engine::core::Vector2& origin,
        std::optional<const engine::core::DepthLayer> layer, const engine::core::GameObject* exclude = nullptr) const
    {
        return find_nearest_object_if<T>(
            origin,exclude,layer,[](const T&) { return true; });
    }

    [[nodiscard]] Character* find_nearest_character(const engine::core::Vector2& origin,
        const engine::core::GameObject* exclude = nullptr) const;

    [[nodiscard]] Enemy* find_nearest_enemy(const engine::core::Vector2& origin) const;

private:
    GameObjectService() = default;

    template <typename T, typename Predicate>
        requires std::derived_from<T, engine::core::GameObject>
    [[nodiscard]] T* find_nearest_object_if(const engine::core::Vector2& origin,
        const engine::core::GameObject* exclude,std::optional<const engine::core::DepthLayer> layer,
        Predicate&& predicate) const
    {
        engine::scene::Scene* scene =engine::scene::SceneManager::instance()->current_scene();

        if (!scene)
            return nullptr;

        T* nearest = nullptr;

        float nearest_distance_squared = std::numeric_limits<float>::max();

        const auto consider_object =[&](engine::core::GameObject& object)
            {
                if (&object == exclude || object.is_destroyed() || !object.is_active())
                    return;

                T* candidate = dynamic_cast<T*>(&object);
                if (!candidate || !predicate(*candidate))
                    return;

                const float distance_squared =(candidate->center() - origin).length_squared();

                if (distance_squared < nearest_distance_squared)
                {
                    nearest = candidate;
                    nearest_distance_squared = distance_squared;
                }
            };

        if (layer)
            scene->for_each_game_object(layer.value(), consider_object);
        else
            scene->for_each_game_object(consider_object);

        return nearest;
    }
};
