# Scene Manager

[Back to the gameplay API guide](README.md)

## What It Does

`SceneManager` owns the scene registry, selects the active scene, and forwards
input, update, render, and ImGui work to it. Scene changes also update the
runtime binding used by the effect system.

## When Gameplay Uses It

Use it for game-flow transitions, such as moving from startup to the menu or
from the menu to a room. Gameplay can also look up a cached scene or request a
scene reset when the flow requires it.

## How to Access It

```cpp
#include "../../engine/scene/scene_manager.h"

engine::scene::SceneManager::instance()->switch_to<RoomScene>();
```

`SceneManager` has no convenience macro.

## Main APIs and Data Types

- `switch_to<T>(args...)` creates or reuses a scene and makes it active.
- `current_scene()` returns the active scene or `nullptr`.
- `try_find_scene<T>()` returns a cached scene or `nullptr`.
- `reset_current_scene()` resets the active scene.
- `reset_scene<T>()` resets a cached scene and reports whether it exists.
- `destroy_scene<T>()` removes a cached scene and reports whether it exists.

`Application` calls `on_input`, `on_update`, `on_render`, `on_imgui`, and
`shutdown`. Gameplay does not need to drive those frame-level methods.

## Typical Workflow

`switch_to<T>()` follows this order:

1. Get the cached scene of type `T`, or construct it on first use.
2. Unbind engine runtime state from the old active scene.
3. Call the old scene's `on_exit()`.
4. Select and bind the new scene.
5. Call the new scene's `on_enter()`.

Calling `switch_to<T>()` when `T` is already active does nothing.

## Ownership and Lifetime

`SceneFactory`, owned by `SceneManager`, stores one `std::unique_ptr<Scene>` per
concrete scene type. A scene remains cached after it becomes inactive. Later
calls to `switch_to<T>()` reuse the same instance.

Constructor arguments are used only when a scene type is first created. New
arguments passed while that type is cached are ignored. Use `reset()` when the
existing instance can rebuild its own state. Destroy the cached scene before
switching when new constructor arguments must be applied.

Pointers returned by `current_scene()` and `try_find_scene<T>()` are borrowed.
Pointers to objects owned by a scene become invalid when that scene destroys or
resets those objects.

## Failure and Empty Results

`current_scene()` returns `nullptr` when no scene is active.
`try_find_scene<T>()` returns `nullptr` when `T` has not been created.
`reset_scene<T>()` and `destroy_scene<T>()` return `false` when `T` is not in
the cache.

Destroying the active scene calls its `on_exit()`, removes it from the cache,
and leaves no active scene.

## Example

```cpp
void MenuScene::start_game()
{
    engine::scene::SceneManager::instance()->switch_to<RoomScene>();
}
```

## Source and Call Sites

- [`SceneManager`](../../engine/scene/scene_manager.h)
- [`SceneManager` lifecycle forwarding](../../engine/scene/scene_manager.cpp)
- [`SceneFactory` cache](../../engine/scene/scene_factory.h)
- [`Scene` lifecycle and object ownership](../../engine/scene/scene.h)
- [`Application` frame loop](../../application/application.cpp)
- [`StartUpLoadingScene` transition](../../gameplay/scene/startup_loading_scene.cpp)
- [`MenuScene` transition](../../gameplay/scene/menu_scene.cpp)
