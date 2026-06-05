#pragma once

#include <SDL.h>

#include "render_command.h"
#include "sdl_convert.h"

#include <vector>

[[nodiscard]] inline SDL_RendererFlip to_sdl_renderer_flip(SpriteFlip flip) noexcept
{
    switch (flip)
    {
    case SpriteFlip::Horizontal:
        return SDL_FLIP_HORIZONTAL;

    case SpriteFlip::Vertical:
        return SDL_FLIP_VERTICAL;

    case SpriteFlip::Both:
        return static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);

    case SpriteFlip::None:
    default:
        return SDL_FLIP_NONE;
    }
}

inline void execute_render_command(SDL_Renderer* renderer, const RenderCommand& render_command) noexcept
{
    if (!renderer || !render_command._texture)
        return;

    SDL_Rect destination_rect = to_sdl_rect(render_command._world_rect);
    SDL_Point rotation_origin{
        static_cast<int>(render_command._rotation_origin.x * destination_rect.w),
        static_cast<int>(render_command._rotation_origin.y * destination_rect.h)
    };

    const SDL_Rect* src_rect = nullptr;
    SDL_Rect converted_src_rect{};
    if (render_command._use_src_rect)
    {
        converted_src_rect = to_sdl_rect(render_command._src_rect);
        src_rect = &converted_src_rect;
    }

    SDL_RenderCopyEx(
        renderer,
        render_command._texture,
        src_rect,
        &destination_rect,
        render_command._rotation_degrees,
        &rotation_origin,
        to_sdl_renderer_flip(render_command._flip)
    );
}

inline void execute_render_commands(
    SDL_Renderer* renderer,
    const std::vector<RenderCommand>& render_commands
) noexcept
{
    for (const RenderCommand& render_command : render_commands)
        execute_render_command(renderer, render_command);
}
