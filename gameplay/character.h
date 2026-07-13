#pragma once

#include "../engine/animation/animation.h"
#include "../engine/animation/effect_manager.h"
#include "../engine/core/game_object.h"
#include "../engine/core/interface/updatable.h"
#include "../engine/core/interface/collidable.h"
#include "../engine/core/interface/kinematic_body.h"
#include "../engine/input/contracts/input_snapshot_receiver.h"

#include "combat/projectile.h"
#include "combat/wand.h"

#include <memory>
#include <string>
#include <vector>

class Character : public engine::core::GameObject,
				  public engine::core::Updatable,
				  public engine::core::Collidable,
				  public engine::core::KinematicBody,
				  public engine::input::InputSnapshotReceiver
{
public:
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

	explicit Character(
		std::string character_id,
		const engine::core::Vector2 &start_position = engine::core::Vector2(300.0f, 300.0f),
		const engine::core::Vector2 &start_size = engine::core::Vector2(100.0f, 100.0f),
		std::string effect_id = {});
	~Character() override;

	void update(double delta) override;
	void on_input_snapshot(const engine::input::InputSnapshot &input) override;
	void submit_render_commands(std::vector<engine::core::RenderCommand> &out_commands) const override;
	std::vector<engine::animation::EffectSpawnRequest> drain_effect_spawn_requests();

	// Wand
	vector<ShotDescriptor> create_projectile(const engine::core::Vector2 &direction);

	void set_move_speed(float move_speed) noexcept;
	void set_hp(float hp) noexcept;
	void set_mana(float mana) noexcept;
	void set_character_size(const engine::core::Vector2 &size);
	void set_position(const engine::core::Vector2 &position);

	[[nodiscard]] engine::core::Vector2 desired_velocity() const noexcept override;
	void apply_translation(const engine::core::Vector2 &delta) noexcept override;

	void die();
	void take_damage(float damage) noexcept;
	bool use_mana(float mana_cost) noexcept;

	// getter
	[[nodiscard]] virtual engine::core::Rect collision_rect() const noexcept;
	[[nodiscard]] const std::string &character_id() const noexcept;
	[[nodiscard]] float move_speed() const noexcept;
	[[nodiscard]] float hp() const noexcept;
	[[nodiscard]] float mana() const noexcept;
	[[nodiscard]] AnimationState animation_state() const noexcept;
	[[nodiscard]] FacingDirection facing_direction() const noexcept;
	[[nodiscard]] bool is_dead() const noexcept;

protected:
	[[nodiscard]] std::string animation_key_for(AnimationState state) const;

private:
	std::string _character_id;
	std::string _effect_id;
	Wand _wand;
	std::vector<engine::animation::EffectSpawnRequest> _pending_effect_requests;
	std::unique_ptr<engine::animation::Animation> _animation;
	AnimationState _animation_state = AnimationState::Idle;
	FacingDirection _facing_direction = FacingDirection::Right;
	engine::core::Vector2 _move_input = engine::core::Vector2::zero();
	engine::core::Vector2 _desired_velocity = engine::core::Vector2::zero();

	float _move_speed = 240.0f;
	float _hp = 100;
	float _mana = 100;

	engine::core::Rect _collision_rect;

	bool _is_dead = false;
};
