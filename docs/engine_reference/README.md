# Gameplay API Guide

This guide shows gameplay developers which engine entry point to use for a
task and who owns the work after the call.

## Before You Call an API

### Singleton access

The APIs in this guide use `Singleton<T>`. Call `T::instance()` to get the
shared instance. Gameplay-facing services also provide convenience macros such
as `GAME_OBJECT_SERVICE`, `AUDIO_SERVICE`, `EFFECT_SERVICE`, and
`PROJECTILE_SERVICE`.

A service call submits work or reads current runtime state. Getting the
singleton does not give the caller ownership of that state.

### `std::optional`

`std::optional<T>` represents a value that may intentionally be absent. The
engine uses it for optional inputs and optional results. An empty input selects
the API's default behavior. An empty result means no value is available.

Check an optional with `if (value)` or `value.has_value()` before using
`*value`. Use `value.value_or(fallback)` when the caller has a real fallback.

### Borrowed pointers

Lookup APIs can return raw pointers. These pointers are borrowed: check for
`nullptr`, use the object without deleting it, and do not keep a gameplay
object pointer after its scene removes or resets the object.

### `[[nodiscard]]`

`[[nodiscard]]` marks a result that affects the next decision. Check the
returned pointer, status, or success value instead of discarding it.

### Ownership and `std::unique_ptr`

`std::unique_ptr` marks exclusive ownership. A scene owns the runtime objects
added to it. Gameplay usually builds a request and lets a service or manager
create and transfer the owned object to the active scene.

## Gameplay-Facing Services

- [Game Object Service](game-object-service.md) queries active gameplay
  objects without owning or caching them.
- [Audio Service](audio-service.md) accepts sound and music requests and owns
  playback scheduling.
- [Effect Service](effect-service.md) turns effect requests into scene-owned
  visual effects.
- [Projectile Service](projectile-service.md) accepts firing intent while the
  projectile manager handles delayed creation and runtime integration.

## Engine Systems Used by Gameplay

- [Scene Manager](scene-manager.md) owns scene instances and controls the active
  scene lifecycle.
- [Resource Manager](resource-manager.md) loads registered assets and provides
  borrowed resource lookups by key.
