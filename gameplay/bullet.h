#pragma once

#include "projectile.h"
#include "../engine/animation/effect_manager.h"

#include <SDL.h>
#include <string>

struct Bullet_Attributes
{
    float bullet_speed = 400.0f;
    engine::core::Vector2 bullet_velocity;

    float max_age = 20.0f;

    engine::core::Vector2 start_position;
    engine::core::Vector2 bullet_size = {24.0f, 24.0f};
    bool damage_based_size = false;

    float curve = 0.0f;
    int bounces = 0;
    float homing_strength = 0;
    bool homing_maintains_speed = true;

    // More damage based on bullet age
    float growth = 40.0f;
    float damage = 100.0f;
};

// Need to seperate entity and map collisison in collidable interface
// All collision treated as wall collisison right now

class Bullet final : public Projectile
{
public:
    Bullet(const Bullet_Attributes &bullet_attributes) noexcept;

    void submit_render_commands(std::vector<engine::core::RenderCommand> &out_commands) const override;
    void on_collision(const engine::core::Vector2 &collision_direction) noexcept override;

    void update(double delta) override;

    inline float get_damage() { return _bullet_attributes.damage; }

private:
    bool handle_wall_bounce(const engine::core::Vector2 &collision_direction);
    engine::animation::EffectSpawnRequest create_collision_effect(const std::string &effect_key);

    void apply_curve(double &delta);
    void apply_growth();
    void apply_damage_sizing();
    void apply_homing(double &delta);

private:
    SDL_Texture *_texture = nullptr;

    Bullet_Attributes _bullet_attributes;
    float _base_damage;
};
