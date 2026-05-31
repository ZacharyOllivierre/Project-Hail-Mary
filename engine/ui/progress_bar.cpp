#include "progress_bar.h"

#include <algorithm>

ProgressBar::ProgressBar(Vector2 position,Vector2 size) :GameObject(DepthLayer::UI)
{
    GameObject::set_world_position(position);
    GameObject::set_size(size);
}


void ProgressBar::on_render(SDL_Renderer* renderer)
{
    const SDL_Rect& object_rect = rect();
    if (!renderer || object_rect.w <= 0 || object_rect.h <= 0)
        return;

    Uint8 old_r = 0;
    Uint8 old_g = 0;
    Uint8 old_b = 0;
    Uint8 old_a = 0;
    SDL_GetRenderDrawColor(renderer, &old_r, &old_g, &old_b, &old_a);

    SDL_SetRenderDrawColor(renderer, _bg_color.r, _bg_color.g, _bg_color.b, _bg_color.a);
    SDL_RenderFillRect(renderer, &object_rect);

    SDL_Rect fill_rect = object_rect;
    fill_rect.w = static_cast<int>(object_rect.w * _progress);
    if (fill_rect.w > 0)
    {
        SDL_SetRenderDrawColor(renderer, _fill_color.r, _fill_color.g, _fill_color.b, _fill_color.a);
        SDL_RenderFillRect(renderer, &fill_rect);
    }

    SDL_SetRenderDrawColor(renderer, old_r, old_g, old_b, old_a);
}

void ProgressBar::reset()
{
    GameObject::reset();
    _progress = 0.0f;
}


void ProgressBar::set_progress(float value)
{
    _progress = std::clamp(value, 0.0f, 1.0f);
}
