#pragma once

#include "character.h"
#include "../../engine/animation/animation.h"
#include "../../engine/animation/effect_manager.h"
#include "../../engine/input/contracts/input_snapshot_receiver.h"
#include "../combat/wand.h"

#include <memory>
#include <string>
#include <vector>

class PlayerCharacter final : public Character,
                              public engine::input::InputSnapshotReceiver
{
public:
    PlayerCharacter(
        std::string character_id,
        const engine::core::Vector2& start_position = engine::core::Vector2(300.0f, 300.0f),
        const engine::core::Vector2& start_size = engine::core::Vector2(100.0f, 100.0f),
        std::string effect_id = {});

    void update(double delta) override;
    void on_input_snapshot(const engine::input::InputSnapshot& input) override;
    void submit_render_commands(
        std::vector<engine::core::RenderCommand>& out_commands) const override;

    [[nodiscard]] std::vector<ShotDescriptor> create_projectile(
        const engine::core::Vector2& direction);
    [[nodiscard]] Wand& wand() noexcept;
    [[nodiscard]] std::vector<engine::animation::EffectSpawnRequest>
        drain_effect_spawn_requests();

protected:
    [[nodiscard]] engine::core::Rect make_body_collision_rect(
        const engine::core::Rect& render_rect) const noexcept override;
    void on_died() noexcept override;

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

private:
    std::string _character_id;
    std::string _effect_id;
    Wand _wand;
    std::vector<engine::animation::EffectSpawnRequest> _pending_effect_requests;
    std::unique_ptr<engine::animation::Animation> _animation;
    AnimationState _animation_state = AnimationState::Idle;
    FacingDirection _facing_direction = FacingDirection::Right;
};
