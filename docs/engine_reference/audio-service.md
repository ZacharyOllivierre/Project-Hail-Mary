# Audio Service

[Back to the gameplay API guide](README.md)

## What It Does

`AudioService` plays music and sound effects, schedules delayed sounds, applies
group policies, tracks stoppable playback with handles, and controls volume.
It resolves registered audio keys through `ResourceManager` and performs the
SDL_mixer calls.

## When Gameplay Uses It

Use it for one-shot sounds, looping sounds, delayed sounds, music changes, or
runtime volume changes. Gameplay supplies a registered key and playback
options; `AudioService` handles channels and scheduling.

## How to Access It

```cpp
#include "../../engine/audio/audio_service.h"

engine::audio::SoundRequestResult result =
    AUDIO_SERVICE->request_sound("bounce");
```

`AUDIO_SERVICE` expands to `engine::audio::AudioService::instance()`.

## Main APIs and Data Types

Sound playback uses these calls:

- `request_sound(key, options)` returns `SoundRequestResult`.
- `play_sound(key, loops)` is a simple immediate-play helper.
- `stop_sound(handle)` stops a pending or active request.
- `cancel_all_scheduled_sounds()` removes delayed requests.
- `stop_all_sounds()` stops every active sound channel.

`SoundPlayOptions` selects the loop count, `SoundGroup`, and start delay.
`SoundRequestResult::status` is `Started`, `Scheduled`, or `Rejected`. Its
`handle` contains a value for started and scheduled requests.

Sound groups control simultaneous playback, per-key cooldown, overflow policy,
and group volume. `set_sound_group_config` returns `false` when the requested
limit exceeds the group's fixed maximum or the cooldown is negative.

Music uses `play_music(key, loops)` and `stop_music()`. Volume setters accept
percent values and clamp them to the range 0 through 100.

## Typical Workflow

1. Choose a sound key registered during resource bootstrap.
2. Select a group and configure loops or delay when needed.
3. Call `request_sound` and inspect its status.
4. Save the optional handle only when the sound may need to be stopped later.

A `Scheduled` result means the request entered the scheduler. The sound can
still be dropped when its due time arrives if cooldown or channel limits block
it.

## Ownership and Lifetime

`Application` initializes `AudioService`, calls `update(delta)` once per frame,
and shuts it down. Gameplay does not drive that lifecycle. `AudioService` owns
the scheduler state; `ResourceManager` owns the `Mix_Chunk` and `Mix_Music`
resources used for playback.

## Failure and Empty Results

`request_sound` returns `Rejected` with an empty handle when the service is not
initialized, the key is missing, or the scheduler rejects an immediate
request. `stop_sound` returns `false` for an unknown or finished handle.
`play_music` returns `false` when the service is unavailable, the key is
missing, or SDL_mixer cannot start playback.

## Example

```cpp
engine::audio::SoundPlayOptions options{
    .loops = -1,
    .group = engine::audio::SoundGroup::Wand,
    .start_delay = std::chrono::milliseconds{0},
};

const engine::audio::SoundRequestResult result =
    AUDIO_SERVICE->request_sound("wand.flight", options);

std::optional<engine::audio::SoundHandle> flight_sound = result.handle;

// Later, when the owning gameplay action ends:
if (flight_sound)
    AUDIO_SERVICE->stop_sound(*flight_sound);
```

## Source and Call Sites

- [`AudioService`](../../engine/audio/audio_service.h)
- [`SoundPlayOptions` and request results](../../engine/audio/sound_playback_types.h)
- [`SoundPlaybackScheduler`](../../engine/audio/sound_playback_scheduler.cpp)
- [`Application` audio lifecycle](../../application/application.cpp)
- [`Bullet` sound requests](../../gameplay/combat/bullet.cpp)
- [`Bullet` behavior sounds](../../gameplay/combat/bullet_behavior/behavior_list.cpp)
