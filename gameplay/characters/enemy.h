#pragma once

#include "character.h"
#include "../../engine/animation/animation.h"
#include "../../engine/physics/collision_box.h"

#include <memory>
#include <string>

class Enemy : public Character
{
public:
    Enemy(std::string character_id,
        const engine::core::Vector2& start_position = engine::core::Vector2(300.0f, 300.0f),
        const engine::core::Vector2& start_size = engine::core::Vector2(100.0f, 100.0f));

    // from Updatable
    // for Scene updates
    void update(double delta) override;

    // from GameObject
    // for Scene rendering
    void submit_render_commands(std::vector<engine::core::RenderCommand>& out_commands) const override;

    // from KinematicBody
    // for PhysicsManager
    [[nodiscard]] engine::core::Vector2 desired_velocity() const noexcept override;
    void apply_translation(const engine::core::Vector2& delta) noexcept override;

    // from Collidable
    // for PhysicsManager
    [[nodiscard]] engine::core::Rect collision_rect() const noexcept override;

    // from CombatReceiver
    // for combat damage delivery
    void receive_attack(const AttackInfo& attack_info) noexcept override;

    // Character lifecycle and state
    void die() noexcept override;
    void set_move_speed(float move_speed) noexcept;
    void set_hp(float hp) noexcept;
    void set_character_size(const engine::core::Vector2& size);
    void set_position(const engine::core::Vector2& position);
    void set_desired_velocity(const engine::core::Vector2& velocity) noexcept;

    // Combat collision
    void set_hurt_collision_box(engine::physics::CollisionBox* collision_box) noexcept;

    [[nodiscard]] float move_speed() const noexcept;
    [[nodiscard]] float hp() const noexcept override;
    [[nodiscard]] bool is_dead() const noexcept override;

protected:
    // from Character
    // for the Enemy body shape
    void refresh_collision_rect() override;
    void update_hurt_collision_box() noexcept;

private:
    enum class AnimationState { Idle, Die };

    void set_animation_state(AnimationState state);

private:
    std::string _character_id;
    std::unique_ptr<engine::animation::Animation> _animation;
    AnimationState _animation_state = AnimationState::Idle;
    float _move_speed = 240.0f;
    float _hp = 100.0f;
    engine::core::Vector2 _desired_velocity = engine::core::Vector2::zero();
    engine::core::Rect _collision_rect{};
    engine::physics::CollisionBox* _hurt_collision_box = nullptr;
    bool _is_dead = false;
};
