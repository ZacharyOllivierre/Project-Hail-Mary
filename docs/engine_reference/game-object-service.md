# Game Object Service

[Back to the gameplay API guide](README.md)

## What It Does

`GameObjectService` finds the nearest matching object in the active scene. It
reads the scene selected by `SceneManager`, performs the query, and returns the
matching object without storing scene or object pointers.

## When Gameplay Uses It

Use it when targeting, aiming, AI, or another gameplay rule needs the nearest
active object. Use the generic typed query for a specific `GameObject` subtype,
or the character and enemy helpers for their built-in gameplay filters.

## How to Access It

```cpp
#include "../object_service/game_object_service.h"

Enemy* target = GAME_OBJECT_SERVICE->find_nearest_enemy(center());
```

`GAME_OBJECT_SERVICE` expands to `GameObjectService::instance()`.

## Main APIs and Data Types

```cpp
template <typename T = engine::core::GameObject>
[[nodiscard]] T* find_nearest_object(
    const engine::core::Vector2& origin,
    const engine::core::GameObject* exclude = nullptr) const;

template <typename T = engine::core::GameObject>
[[nodiscard]] T* find_nearest_object(
    const engine::core::Vector2& origin,
    std::optional<const engine::core::DepthLayer> layer,
    const engine::core::GameObject* exclude = nullptr) const;

[[nodiscard]] Character* find_nearest_character(
    const engine::core::Vector2& origin,
    const engine::core::GameObject* exclude = nullptr) const;

[[nodiscard]] Enemy* find_nearest_enemy(
    const engine::core::Vector2& origin) const;
```

The generic query accepts any type derived from `GameObject`. The layer
overload traverses only that depth layer. Passing an empty optional traverses
all layers.

## Typical Workflow

1. Choose the world-space origin for the search.
2. Choose the expected object type and an optional depth layer.
3. Pass the caller as `exclude` when it must not select itself.
4. Check the returned pointer and use it for the current operation.

Every query skips destroyed and inactive objects. Typed queries use
`dynamic_cast` and compare squared center-to-center distance. Character and
enemy helpers also skip dead objects. `find_nearest_character` accepts an
object to exclude; `find_nearest_enemy` does not.

## Ownership and Lifetime

The active `Scene` owns every returned object. `GameObjectService` owns
nothing and keeps no cache. A returned pointer can become invalid when the
object is destroyed, the scene resets, or the active scene changes.

## Failure and Empty Results

The query returns `nullptr` when there is no active scene or no eligible
object. An invalid depth layer also produces no result because the scene has no
container to traverse for that layer.

## Example

```cpp
Character* nearest = GAME_OBJECT_SERVICE->find_nearest_character(
    center(),
    this);

if (nearest)
{
    const engine::core::Vector2 direction =
        (nearest->center() - center()).normalized();
    aim_at(direction);
}
```

## Source and Call Sites

- [`GameObjectService`](../../gameplay/object_service/game_object_service.h)
- [`GameObjectService` helpers](../../gameplay/object_service/game_object_service.cpp)
- [`Scene` object storage and traversal](../../engine/scene/scene.h)
- [`SceneManager::current_scene`](../../engine/scene/scene_manager.h)
