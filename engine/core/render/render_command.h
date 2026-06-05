#pragma once

#include <SDL.h>

#include "../geometry/rect.h"
#include "../geometry/vector2.h"

enum class SpriteFlip
{
    None,
    Horizontal,
    Vertical,
    Both
};

struct RenderCommand
{
    SDL_Texture* texture = nullptr;

    // Destination rectangle in world space.
    // This will be transformed by the camera before rendering.
    Rect world_rect{};

    // Source rectangle in texture space.
    // Defines which part of the texture or sprite sheet should be drawn.
    bool use_src_rect = false;
    Rect src_rect{};

    // Clockwise rotation angle in degrees.
    double rotation_degrees = 0.0;

    // Normalized rotation origin inside _world_rect.
    // (0.5, 0.5) -> center, (0, 0) -> top-left
    Vector2 rotation_origin = Vector2(0.5f, 0.5f);

    SpriteFlip flip = SpriteFlip::None;
};

struct UIRenderCommand
{
    SDL_Texture* texture = nullptr;

    // Destination rectangle in screen/UI space.
    // This is not affected by the world camera.
    Rect screen_rect{};

    // Source rectangle in texture space.
    // Defines which part of the texture or sprite sheet should be drawn.
    // If false, the full texture is drawn.
    bool use_src_rect = false;
    Rect src_rect{};

    // Clockwise rotation angle in degrees.
    double rotation_degrees = 0.0;

    // Normalized rotation origin inside _screen_rect.
    // (0.5, 0.5) -> center, (0, 0) -> top-left
    Vector2 rotation_origin = Vector2(0.5f, 0.5f);

    SpriteFlip flip = SpriteFlip::None;
};
