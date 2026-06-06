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

inline void execute_textured_render_command(
    SDL_Renderer* renderer,
    SDL_Texture* texture,
    const Rect& destination_rect_value,
    Uint8 alpha,
    bool use_src_rect,
    const Rect& src_rect_value,
    double rotation_degrees,
    const Vector2& rotation_origin,
    SpriteFlip flip
) noexcept
{
    if (!renderer || !texture)
        return;

    SDL_Rect destination_rect = to_sdl_rect(destination_rect_value);
    SDL_Point sdl_rotation_origin{
        static_cast<int>(rotation_origin.x * destination_rect.w),
        static_cast<int>(rotation_origin.y * destination_rect.h)
    };

    const SDL_Rect* src_rect = nullptr;
    SDL_Rect converted_src_rect{};
    if (use_src_rect)
    {
        converted_src_rect = to_sdl_rect(src_rect_value);
        src_rect = &converted_src_rect;
    }

    Uint8 previous_alpha = 255;
    SDL_GetTextureAlphaMod(texture, &previous_alpha);
    SDL_SetTextureAlphaMod(texture, alpha);

    SDL_RenderCopyEx(
        renderer,
        texture,
        src_rect,
        &destination_rect,
        rotation_degrees,
        &sdl_rotation_origin,
        to_sdl_renderer_flip(flip)
    );

    SDL_SetTextureAlphaMod(texture, previous_alpha);
}

inline void execute_render_command(SDL_Renderer* renderer, const RenderCommand& render_command) noexcept
{
    execute_textured_render_command(
        renderer,
        render_command.texture,
        render_command.command_rect,
        render_command.alpha,
        render_command.use_src_rect,
        render_command.src_rect,
        render_command.rotation_degrees,
        render_command.rotation_origin,
        render_command.flip
    );
}

inline void execute_render_command(SDL_Renderer* renderer, const UiRenderCommand& render_command) noexcept
{
    execute_textured_render_command(
        renderer,
        render_command.texture,
        render_command.screen_rect,
        render_command.alpha,
        render_command.use_src_rect,
        render_command.src_rect,
        render_command.rotation_degrees,
        render_command.rotation_origin,
        render_command.flip
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

inline void execute_render_commands(
    SDL_Renderer* renderer,
    const std::vector<UiRenderCommand>& render_commands
) noexcept
{
    for (const UiRenderCommand& render_command : render_commands)
        execute_render_command(renderer, render_command);
}
