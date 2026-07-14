#pragma once

#include "../../engine/core/game_object.h"
#include "../../engine/core/interface/collidable.h"
#include "../../engine/core/interface/kinematic_body.h"
#include "../../engine/core/interface/updatable.h"
#include "../combat/attack_info.h"

class Character : public engine::core::GameObject, public engine::core::Updatable,
                  public engine::core::Collidable,public engine::core::KinematicBody,
                  public CombatReceiver
{
public:
    explicit Character(
        const engine::core::Vector2& start_position = engine::core::Vector2(300.0f, 300.0f),
        const engine::core::Vector2& start_size = engine::core::Vector2(100.0f, 100.0f));
    ~Character() override;

    //from updateable
    void update(double delta) override;

    //from KinematicBody
    //for PhysicsManager
    [[nodiscard]] engine::core::Vector2 desired_velocity() const noexcept override;
    void apply_translation(const engine::core::Vector2& delta) noexcept override;

    //from CombatReceiver
    //for CollisionManager
    void receive_attack(const AttackInfo& attack_info) noexcept override;

    //from Collidable
    //for PhysicsManager
    [[nodiscard]] engine::core::Rect collision_rect() const noexcept override;

    [[nodiscard]] engine::core::Rect collision_rect() const noexcept override;
    [[nodiscard]] engine::core::Rect hurt_rect() const noexcept override;

    void die() noexcept;
    void set_move_speed(float move_speed) noexcept;
    void set_hp(float hp) noexcept;
    void set_character_size(const engine::core::Vector2& size);
    void set_position(const engine::core::Vector2& position);
    void set_desired_velocity(const engine::core::Vector2& velocity) noexcept;

    [[nodiscard]] float move_speed() const noexcept;
    [[nodiscard]] float hp() const noexcept;
    [[nodiscard]] bool is_dead() const noexcept;

private:
    float _move_speed = 240.0f;
    float _hp = 100.0f;
    
    engine::core::Vector2 _desired_velocity = engine::core::Vector2::zero();

    engine::core::Rect _hurt_rect{};//CollisionManager
    engine::core::Rect _collision_rect{};//PhysicsManager
    bool _is_dead = false;
};
