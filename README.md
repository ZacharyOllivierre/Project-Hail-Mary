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

Install CMake and the MSVC Build Tools automatically, then build the project:

```powershell
powershell -ExecutionPolicy Bypass -File .\environment\setup-windows.ps1
```

```bash
cmake -S . -B build -A x64
cmake --build build --config Release
./build/Release/Hail.exe
```

### macOS

Install the SDL2 dependencies first:

Install Homebrew, CMake, and every SDL2 dependency automatically, then build the project:

```bash
bash environment/setup-macos.sh
```

To install dependencies manually:

```bash
brew install sdl2 sdl2_image sdl2_net sdl2_mixer sdl2_ttf
```

Then configure and build:

```bash
cmake -S . -B build
cmake --build build
```

## Run the Game

From the repository root, run:

```bash
./build/Hail
```

If you are already in the `build/` directory, run:

```bash
./Hail
```

## Tests

From the repository root, run:

```bash
ctest --test-dir build --output-on-failure
```

If you are already in the `build/` directory, run:

```bash
ctest --output-on-failure
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

## Licensing

Project-authored source code, build files, tests, documentation, and editable
asset configuration files are licensed under
[GPL-3.0-only](LICENSE). Protected game assets are not covered by that grant:
see [the assets license](assets/LICENSE.md). Third-party dependencies retain
their own licenses; see [Third-Party Notices](THIRD_PARTY_NOTICES.md).

# Project-Hail-Mary

## Build on WSL Ubuntu

Linux builds use the system SDL2 development packages. The bundled SDL2
libraries under `thirdparty/SDL2/lib` are Windows libraries and are not used
by WSL.

Install the required packages in WSL Ubuntu:

```bash
sudo apt update
sudo apt install build-essential cmake pkg-config \
  libsdl2-dev libsdl2-net-dev libsdl2-gfx-dev \
  libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```

Configure and build from a separate Linux build directory so that the Linux
and Windows CMake caches do not get mixed:

```bash
cmake -S . -B build-linux -G "Unix Makefiles" \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build-linux -j"$(nproc)"
ctest --test-dir build-linux --output-on-failure
```

The Linux CMake configuration discovers `sdl2`, `SDL2_net`, `SDL2_gfx`,
`SDL2_image`, `SDL2_mixer`, and `SDL2_ttf` through `pkg-config`. ImGui remains
enabled and is built from `thirdparty/imgui`.
