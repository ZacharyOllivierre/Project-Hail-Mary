#pragma once

#include <SDL.h>

#include "../geometry/rect.h"
#include "../geometry/vector2.h"

[[nodiscard]] inline SDL_Point to_sdl_point(const Vector2& point) noexcept
{
    return SDL_Point{
        static_cast<int>(point.x),
        static_cast<int>(point.y)
    };
}

[[nodiscard]] inline SDL_Rect to_sdl_rect(const Rect& rect) noexcept
{
    SDL_Rect sdl_rect{};
    sdl_rect.x = static_cast<int>(rect.x());
    sdl_rect.y = static_cast<int>(rect.y());

    const int width = static_cast<int>(rect.width());
    const int height = static_cast<int>(rect.height());
    sdl_rect.w = width > 0 ? width : 0;
    sdl_rect.h = height > 0 ? height : 0;
    return sdl_rect;
}

[[nodiscard]] inline SDL_FRect to_sdl_frect(const Rect& rect) noexcept
{
    SDL_FRect sdl_rect{};
    sdl_rect.x = rect.x();
    sdl_rect.y = rect.y();
    sdl_rect.w = rect.width();
    sdl_rect.h = rect.height();
    return sdl_rect;
}
