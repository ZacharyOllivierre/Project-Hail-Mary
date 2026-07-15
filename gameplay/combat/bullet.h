#pragma once

#include "wand_types.h"
#include "projectile.h"
#include "../../engine/animation/effect_manager.h"

#include <SDL.h>
#include <string>

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
    void apply_acceleration(double &delta);

private:
    SDL_Texture *_texture = nullptr;

    Bullet_Attributes _bullet_attributes;
    float _base_damage;
};
