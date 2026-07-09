#pragma once

#include "projectile.h"

#include <SDL.h>

class Bullet final : public Projectile
{
public:
    Bullet(
        const Vector2 &start_position,
        const Vector2 &start_velocity) noexcept;

    void submit_render_commands(std::vector<RenderCommand> &out_commands) const override;
    void on_collision() noexcept override;

private:
    SDL_Texture *_texture = nullptr;
};