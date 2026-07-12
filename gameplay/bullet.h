#pragma once

#include "projectile.h"

#include <SDL.h>

struct Bullet_Attributes
{
    float damage = 100.0f;

    float bullet_speed = 300.0f;
    Vector2 bullet_velocity;

    Vector2 bullet_size = {24.0f, 24.0f};

    float curve = 0.0f;
    int bounces = 3;

    // More damage based on bullet age
    float growth = 0.0f;
};

// Need to seperate entity and map collisison in collidable interface
// ^ implement damage
// All collision treated as entity collisison right now

class Bullet final : public Projectile
{
public:
    Bullet(const Bullet_Attributes &bullet_attributes, const Vector2 &start_position) noexcept;

    void submit_render_commands(std::vector<RenderCommand> &out_commands) const override;
    void on_collision(const Vector2 &collision_direction) noexcept override;

    void update(double delta) override;

    inline float get_damage() { return _bullet_attributes.damage; }

private:
    SDL_Texture *_texture = nullptr;

    Bullet_Attributes _bullet_attributes;
};