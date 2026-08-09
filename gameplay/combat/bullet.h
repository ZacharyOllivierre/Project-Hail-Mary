#pragma once

#include "wand_types.h"
#include "projectile.h"
#include "attack_info.h"
#include "bullet_behavior/bullet_behavior_set.h"

#include <SDL.h>
#include <string>
#include <unordered_map>

class Bullet final : public Projectile
{
public:
    Bullet(const Bullet_Attributes &bullet_attributes) noexcept;

    void submit_render_commands(std::vector<engine::core::RenderCommand> &out_commands) const override;

    void on_collision(const engine::core::Vector2 &collision_direction) noexcept override;
    void on_entity_collision(GameObject *entity) noexcept override;
    void on_destroy() noexcept override;

    void update(double delta) override;

    AttackInfo attack_info() const noexcept override;
    bool can_hit(GameObject *object) const noexcept override;

    // Temporary fix to expose bullet attributes to behaviors (see curve behavior.cpp for comment)
    inline Bullet_Attributes *get_bullet_attributes() { return &_bullet_attributes; }

    BulletBehaviorSet *behavior_set() { return &_behaviors; }

private:
    void update_hit_cooldowns(double &delta);

private:
    SDL_Texture *_texture = nullptr;

    Bullet_Attributes _bullet_attributes;
    float _base_damage;
    BulletBehaviorSet _behaviors;

    // Collision spam fix prevents repeated damage every frame from pierce
    std::unordered_map<GameObject *, float> _hit_cooldowns;
};
