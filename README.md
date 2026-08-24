# Project Hail Mary

[English](README.md) | [中文](README_CN.md)

A 2D top-down action roguelike prototype built with C++23 and SDL2.

Project Hail Mary currently delivers a playable combat slice: boot into a menu, enter a generated room, move, aim, fire projectiles, and fight multiple enemy types. Its long-term direction is room-based dungeon progression and build-driven combat.

## Status

**Stage:** Early playable vertical slice.

**Working now:** application lifecycle, scene transitions, configuration-driven assets, generated rooms, player control, projectile combat, five enemy types, basic UI, sound, and effects.

**Still in development:** complete room-to-room progression, win and loss states, HUD and result screens, persistence, wider content, and broader automated testing.

For the complete production assessment, read [Project Status](introduction/project_status_en.md).

## Controls

| Input | Action |
| --- | --- |
| `WASD` or arrow keys | Move |
| Left mouse button or `J` | Attack |
| `Enter` | Confirm / start |
| `R` | Reset the current room |

## Build

### Windows

The repository includes the required SDL2-related runtime libraries for the bundled Windows build configuration. Use Visual Studio 2022 or newer with a 64-bit generator.

```bash
cmake -S . -B build -A x64
cmake --build build --config Release
./build/Release/Hail.exe
```

### macOS

Install the SDL2 dependencies first:

```bash
brew install sdl2 sdl2_image sdl2_net sdl2_mixer sdl2_ttf
```

Then configure, build, and run:

```bash
cmake -S . -B build
cmake --build build
./build/Hail
```

## Tests

```bash
ctest --test-dir build --output-on-failure
```

Current tests provide basic regression coverage; they are not yet a complete gameplay validation suite.

## Repository Layout

```text
application/   Application lifecycle and main loop
engine/        Reusable systems: input, resources, audio, scenes, physics, UI
gameplay/      Characters, combat, map generation, enemy spawning, and game scenes
assets/        Textures, fonts, audio, preload assets, and configuration
docs/          Project documentation
tests/         Automated tests
thirdparty/    Third-party dependencies
```

## Documentation

- [English project status](introduction/project_status_en.md)
- [Chinese project status](introduction/project_status_zh_cn.md)
- [Chinese README](README_CN.md)
- [Gameplay narrative pitch](introduction/Gameplay%20Narrative%20Pitch.pdf)
- [Coding standard](docs/coding_standard.md)