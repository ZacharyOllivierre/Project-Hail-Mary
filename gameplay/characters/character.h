#pragma once

#include "../../engine/core/game_object.h"
#include "../../engine/core/interface/collidable.h"
#include "../../engine/core/interface/kinematic_body.h"
#include "../../engine/core/interface/updatable.h"
#include "../combat/attack_info.h"
#include "../combat/status_effect_manager.h"

namespace engine::physics
{
class CollisionBox;
}

class Character : public engine::core::GameObject,
                  public engine::core::Updatable,
                  public engine::core::Collidable,
                  public engine::core::KinematicBody,
                  public CombatReceiver
{
public:
    ~Character() override = default;

    void update(double delta) override;

    [[nodiscard]] engine::core::Vector2 desired_velocity() const noexcept override;
    void apply_translation(const engine::core::Vector2& delta) noexcept override;
    [[nodiscard]] engine::core::Rect collision_rect() const noexcept override;

    void receive_attack(const AttackInfo& attack_info) noexcept override;
    virtual void die() noexcept;

    void set_move_speed(float move_speed) noexcept;
    void set_hp(float hp) noexcept;
    void set_character_size(const engine::core::Vector2& size);
    void set_position(const engine::core::Vector2& position);
    void set_desired_velocity(const engine::core::Vector2& velocity) noexcept;

    [[nodiscard]] float move_speed() const noexcept;
    [[nodiscard]] virtual float hp() const noexcept;
    [[nodiscard]] virtual bool is_dead() const noexcept;

    virtual void submit_render_commands(
        std::vector<engine::core::RenderCommand>& out_commands) const override = 0;

protected:
    Character(
        const engine::core::Vector2& start_position,
        const engine::core::Vector2& start_size) noexcept;

    void set_hurt_collision_box(engine::physics::CollisionBox* collision_box) noexcept;
    void refresh_collision_shapes() noexcept;

    [[nodiscard]] virtual engine::core::Rect make_body_collision_rect(
        const engine::core::Rect& render_rect) const noexcept = 0;
    [[nodiscard]] virtual engine::core::Rect make_hurt_box_rect(
        const engine::core::Rect& render_rect) const noexcept;
    virtual void on_died() noexcept = 0;

    [[nodiscard]] static engine::core::Rect make_bottom_centered_scaled_rect(
        const engine::core::Rect& source,
        const engine::core::Vector2& scale) noexcept;
    [[nodiscard]] static engine::core::Rect make_centered_scaled_rect(
        const engine::core::Rect& source,
        const engine::core::Vector2& scale) noexcept;

private:
    void add_status_effect(const std::shared_ptr<StatusEffect>& effect) noexcept;
    void clear_status_effects() noexcept;

private:
    StatusEffectManager _status_effect_manager;
    engine::core::Vector2 _desired_velocity = engine::core::Vector2::zero();
    engine::core::Rect _collision_rect{};
    engine::physics::CollisionBox* _hurt_collision_box = nullptr;
    float _move_speed = 240.0f;
    float _hp = 100.0f;
    bool _is_dead = false;
};
