#!/usr/bin/env bash
set -euo pipefail

configuration="${1:-Release}"
if [[ "$configuration" != "Debug" && "$configuration" != "Release" ]]; then
    printf 'Configuration must be Debug or Release.\n' >&2
    exit 1
fi

project_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

if ! command -v brew >/dev/null 2>&1; then
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
fi

if ! command -v brew >/dev/null 2>&1; then
    if [[ -x /opt/homebrew/bin/brew ]]; then
        eval "$(/opt/homebrew/bin/brew shellenv)"
    elif [[ -x /usr/local/bin/brew ]]; then
        eval "$(/usr/local/bin/brew shellenv)"
    else
        printf 'Homebrew installation completed but brew is not available in this shell. Open a new terminal and run this script again.\n' >&2
        exit 1
    fi
fi

required_formulae=(cmake sdl2 sdl2_image sdl2_net sdl2_mixer sdl2_ttf)
missing_formulae=()

for formula in "${required_formulae[@]}"; do
    if ! brew list --versions "$formula" >/dev/null 2>&1; then
        missing_formulae+=("$formula")
    fi
done

if (( ${#missing_formulae[@]} > 0 )); then
    HOMEBREW_NO_AUTO_UPDATE=1 \
    HOMEBREW_NO_INSTALLED_DEPENDENTS_CHECK=1 \
        brew install --no-ask "${missing_formulae[@]}"
fi

build_directory="$project_root/build/macos"
cmake -S "$project_root" -B "$build_directory" \
    -DCMAKE_BUILD_TYPE="$configuration" \
    -DCMAKE_PREFIX_PATH="$(brew --prefix)" \
    -DBUILD_TESTING=ON
cmake --build "$build_directory"

printf 'Build complete: %s/Hail\n' "$build_directory"