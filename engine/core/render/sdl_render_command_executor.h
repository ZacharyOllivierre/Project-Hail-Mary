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
    if (!renderer)
        return;

    SDL_Rect previous_clip_rect{};
    const bool had_clip_rect = SDL_RenderIsClipEnabled(renderer) == SDL_TRUE;
    if (had_clip_rect)
        SDL_RenderGetClipRect(renderer, &previous_clip_rect);

    if (render_command.use_clip_rect)
    {
        const SDL_Rect clip_rect = to_sdl_rect(render_command.clip_rect);
        SDL_RenderSetClipRect(renderer, &clip_rect);
    }
    else if (had_clip_rect)
    {
        SDL_RenderSetClipRect(renderer, nullptr);
    }

    switch (render_command.type)
    {
    case UiRenderCommandType::Texture:
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
        break;

    case UiRenderCommandType::FillRect:
    case UiRenderCommandType::DrawRect:
    {
        if (render_command.screen_rect.is_empty())
            break;

        SDL_Rect rect = to_sdl_rect(render_command.screen_rect);

        Uint8 old_r = 0;
        Uint8 old_g = 0;
        Uint8 old_b = 0;
        Uint8 old_a = 0;
        SDL_GetRenderDrawColor(renderer, &old_r, &old_g, &old_b, &old_a);

        SDL_SetRenderDrawColor(
            renderer,
            render_command.color.r,
            render_command.color.g,
            render_command.color.b,
            render_command.color.a
        );

        if (render_command.type == UiRenderCommandType::FillRect)
            SDL_RenderFillRect(renderer, &rect);
        else
            SDL_RenderDrawRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, old_r, old_g, old_b, old_a);
        break;
    }

    case UiRenderCommandType::DrawLine:
    {
        Uint8 old_r = 0;
        Uint8 old_g = 0;
        Uint8 old_b = 0;
        Uint8 old_a = 0;
        SDL_GetRenderDrawColor(renderer, &old_r, &old_g, &old_b, &old_a);

        SDL_SetRenderDrawColor(
            renderer,
            render_command.color.r,
            render_command.color.g,
            render_command.color.b,
            render_command.color.a
        );

        const SDL_Point start = to_sdl_point(render_command.line_start);
        const SDL_Point end = to_sdl_point(render_command.line_end);
        SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);

        SDL_SetRenderDrawColor(renderer, old_r, old_g, old_b, old_a);
        break;
    }
    }

    if (had_clip_rect)
        SDL_RenderSetClipRect(renderer, &previous_clip_rect);
    else
        SDL_RenderSetClipRect(renderer, nullptr);
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
