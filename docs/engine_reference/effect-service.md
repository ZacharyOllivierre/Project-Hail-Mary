# Effect Service

[Back to the gameplay API guide](README.md)

## What It Does

`EffectService` accepts visual-effect spawn requests from gameplay.
`EffectManager` resolves the registered definition, creates the runtime effect,
and transfers it to the active scene.

## When Gameplay Uses It

Use it when an attack, hit, movement, or other gameplay event needs a visual
effect at a world position. Gameplay describes the effect through a request
and does not create or store the runtime effect object.

## How to Access It

```cpp
#include "../../engine/effects/effect_service.h"

const bool spawned =
    EFFECT_SERVICE->request_animation_effect(request);
```

`EFFECT_SERVICE` expands to `engine::effects::EffectService::instance()`.

## Main APIs and Data Types

```cpp
[[nodiscard]] bool request_animation_effect(
    const AnimationEffectSpawnRequest& request);

[[nodiscard]] bool request_floating_number_effect(
    const FloatingNumberEffectSpawnRequest& request);
```

`AnimationEffectSpawnRequest` contains:

- `effect_key` for the registered effect definition.
- `position` and `anchor` for world placement.
- Optional `size`, `angle_degrees`, and `flip` overrides.
- `start_delay_seconds` for delayed playback.
- `on_started`, `on_finished`, and timed callbacks.

When no size override is present, the factory uses the registered default size
and then the current animation frame size. Angle and flip also fall back to
their registered or built-in defaults.

`request_floating_number_effect` currently returns `false`. Floating-number
requests are not dispatched to a runtime manager yet.

## Typical Workflow

1. Choose an effect key loaded during resource bootstrap.
2. Fill the world position and anchor.
3. Add optional presentation overrides or callbacks.
4. Submit the request and handle the returned `bool`.

## Ownership and Lifetime

`SceneManager` binds `EffectManager` to the active scene before the scene's
`on_enter()` call. The manager creates an `AnimationEffect` and moves its
`std::unique_ptr` into that scene. The scene updates, renders, and destroys the
effect when playback finishes.

The manager unbinds the scene before `on_exit()`, so an effect request made
from `on_exit()` fails.

## Failure and Empty Results

`request_animation_effect` returns `false` when no scene is active, the effect
key has no registered definition, the animation cannot be created, or the
scene rejects the new object.

## Example

```cpp
engine::effects::AnimationEffectSpawnRequest request;
request.effect_key = "fire.impact_radial";
request.position = center();
request.anchor = engine::effects::EffectAnchor::Center;
request.size = engine::core::Vector2{200.0f, 200.0f};

if (!EFFECT_SERVICE->request_animation_effect(request))
{
    ENGINE_LOG_WARN("gameplay", "Failed to spawn fire impact effect.");
}
```

## Source and Call Sites

- [`EffectService`](../../engine/effects/effect_service.h)
- [`Effect request types`](../../engine/effects/effect_types.h)
- [`EffectManager`](../../engine/effects/runtime/effect_manager.cpp)
- [`AnimationEffectFactory`](../../engine/effects/animation/animation_effect_factory.cpp)
- [`SceneManager` effect binding](../../engine/scene/scene_manager.cpp)
- [`PlayerCharacter` effect request](../../gameplay/characters/player_character.cpp)
