#pragma once

#include "character.h"
#include "../../engine/animation/animation.h"
#include "../../engine/animation/effect_manager.h"
#include "../../engine/input/contracts/input_snapshot_receiver.h"
#include "../../engine/physics/collision_box.h"
#include "../combat/wand.h"

#include <memory>
#include <string>
#include <vector>

class PlayerCharacter final : public Character, public engine::input::InputSnapshotReceiver
{
public:
    PlayerCharacter(std::string character_id,
                    const engine::core::Vector2 &start_position = engine::core::Vector2(300.0f, 300.0f),
                    const engine::core::Vector2 &start_size = engine::core::Vector2(100.0f, 100.0f),
                    std::string effect_id = {});

    // from Updatable
    // for Scene updates
    void update(double delta) override;

    // from InputSnapshotReceiver
    // for player controls
    void on_input_snapshot(const engine::input::InputSnapshot &input) override;

    // from GameObject
    // for Scene rendering
    void submit_render_commands(std::vector<engine::core::RenderCommand> &out_commands) const override;

    // from KinematicBody
    // for PhysicsManager
    [[nodiscard]] engine::core::Vector2 desired_velocity() const noexcept override;
    void apply_translation(const engine::core::Vector2 &delta) noexcept override;

    // from Collidable
    // for PhysicsManager
    [[nodiscard]] engine::core::Rect collision_rect() const noexcept override;

    // from CombatReceiver
    // for combat damage delivery
    void receive_attack(const AttackInfo &attack_info) noexcept override;

    // Character lifecycle and state
    void die() noexcept override;
    void set_move_speed(float move_speed) noexcept;
    void set_hp(float hp) noexcept;
    void set_character_size(const engine::core::Vector2 &size);
    void set_position(const engine::core::Vector2 &position);
    void set_desired_velocity(const engine::core::Vector2 &velocity) noexcept;

    // Combat collision
    void set_hurt_collision_box(engine::physics::CollisionBox *collision_box) noexcept;

    [[nodiscard]] float move_speed() const noexcept;
    [[nodiscard]] float hp() const noexcept override;
    [[nodiscard]] bool is_dead() const noexcept override;

    // Player combat
    [[nodiscard]] std::vector<ShotDescriptor> create_projectile(const engine::core::Vector2 &direction);
    [[nodiscard]] Wand &wand() noexcept;
    [[nodiscard]] std::vector<engine::animation::EffectSpawnRequest> drain_effect_spawn_requests();

private:
    enum class AnimationState
    {
        Idle,
        Move,
        Die
    };
    enum class FacingDirection
    {
        Right,
        Left
    };

    void set_animation_state(AnimationState state);

protected:
    // from Character
    // for the PlayerCharacter body shape
    void refresh_collision_rect() override;
    void update_hurt_collision_box() noexcept;

private:
    std::string _character_id;
    std::string _effect_id;
    Wand _wand;

    std::vector<engine::animation::EffectSpawnRequest> _pending_effect_requests;

    std::unique_ptr<engine::animation::Animation> _animation;
    AnimationState _animation_state = AnimationState::Idle;
    FacingDirection _facing_direction = FacingDirection::Right;

    engine::core::Vector2 _desired_velocity = engine::core::Vector2::zero();
    engine::core::Rect _collision_rect{};
    engine::physics::CollisionBox *_hurt_collision_box = nullptr;

    float _move_speed = 240.0f;
    float _hp = 100.0f;
    bool _is_dead = false;
};
