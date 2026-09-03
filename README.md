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
