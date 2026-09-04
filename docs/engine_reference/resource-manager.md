# Resource Manager

[Back to the gameplay API guide](README.md)

## What It Does

`ResourceManager` loads the registered fonts, sounds, music, textures,
animation atlases, and effect definitions used by the game. Gameplay retrieves
loaded resources by string key.

## When Gameplay Uses It

Gameplay uses `find_*` calls when an object or scene needs a texture, font,
sound, music track, or atlas. Startup loading uses `init` and the `load_*`
functions to populate those stores.

## How to Access It

```cpp
#include "../../engine/resources/resource_manager.h"

SDL_Texture* texture =
    engine::resources::ResourceManager::instance()->find_texture("bullet");
```

`ResourceManager` has no convenience macro.

## Main APIs and Data Types

Gameplay lookup APIs are:

- `find_font(key)` returns `TTF_Font*`.
- `find_sound(key)` returns `Mix_Chunk*`.
- `find_music(key)` returns `Mix_Music*`.
- `find_texture(key)` returns `SDL_Texture*`.
- `find_atlas(key)` returns `const Atlas*`.

Loading APIs are `init(renderer)`, `load_font`, `load_sound`, `load_music`, and
`load_texture`. `ResourceBootstrapper` calls the loading APIs from the startup
flow using the manifests registered in `assets/configs_list.json`.

## Typical Workflow

Startup loading follows this path:

1. `StartUpLoadingScene` renders the preload image and captures the renderer.
2. Its next update calls `ResourceManager::init(renderer)` on the main thread.
3. `ResourceBootstrapper` reads the config registry and loads every required
   resource group.
4. The loading scene switches to `MenuScene` after bootstrap succeeds.

Normal gameplay looks up an already registered key, checks the returned
pointer, and stores it only for as long as the resource manager remains alive.

## Ownership and Lifetime

`ResourceManager` owns the texture, font, audio, and atlas managers. Those
managers own the SDL resource objects in their stores. Every pointer returned
by `find_*` is borrowed; callers do not free it.

The manager lives for the process lifetime through `Singleton<T>`. The normal
flow loads each key once during startup, so gameplay can keep a successful
lookup for the runtime. Reloading an existing font, sound, or music key replaces
its native resource and invalidates pointers returned for the old value.

## Failure and Empty Results

Every `find_*` function returns `nullptr` when the key is not registered.
Gameplay should handle that result before rendering, measuring text, or
passing the resource to another engine API.

`init` returns `false` when the renderer is null, project paths cannot be
resolved, a required manifest cannot be read, or any required resource fails
to load. The startup loading scene keeps the failure state instead of entering
the menu.

## Example

```cpp
_texture = engine::resources::ResourceManager::instance()->find_texture(
    "bullet");

if (!_texture)
{
    ENGINE_LOG_WARN("gameplay", "Missing bullet texture.");
    return;
}
```

## Source and Call Sites

- [`ResourceManager`](../../engine/resources/resource_manager.h)
- [`ResourceManager` implementation](../../engine/resources/resource_manager.cpp)
- [`ResourceBootstrapper`](../../engine/resources/resource_bootstrapper.cpp)
- [`Resource config registry`](../../assets/configs_list.json)
- [`StartUpLoadingScene` initialization](../../gameplay/scene/startup_loading_scene.cpp)
- [`MenuScene` font lookup](../../gameplay/scene/menu_scene.cpp)
- [`Bullet` texture lookup](../../gameplay/combat/bullet.cpp)
