#pragma once

#include "../engine/animation/animation.h"
#include "../engine/animation/effect_manager.h"
#include "../engine/core/game_object.h"
#include "../engine/core/interface/updatable.h"
#include "../engine/core/interface/collidable.h"
#include "../engine/core/interface/kinematic_body.h"
#include "../engine/input/contracts/input_snapshot_receiver.h"

#include <memory>
#include <string>
#include <vector>

class Character : public GameObject, 
	public Updatable, public Collidable, public KinematicBody,
	public InputSnapshotReceiver
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
		const Vector2& start_position = Vector2::zero(),
		const Vector2& start_size = Vector2(100.0f, 100.0f),
		std::string effect_id = {}
	);
	~Character() override;

	void update(double delta) override;
	void on_input_snapshot(const InputSnapshot& input) override;
	void submit_render_commands(std::vector<RenderCommand>& out_commands) const override;
	std::vector<EffectSpawnRequest> drain_effect_spawn_requests();

	void set_move_speed(float move_speed) noexcept;
	void set_hp(float hp) noexcept;
	void set_mana(float mana) noexcept;
	void set_character_size(const Vector2& size);
	void set_position(const Vector2& position);

	[[nodiscard]] Vector2 desired_velocity() const noexcept override;
	void apply_translation(const Vector2& delta) noexcept override;

	void die();
	void take_damage(float damage) noexcept;
	bool use_mana(float mana_cost) noexcept;

	//getter
	[[nodiscard]] virtual Rect collision_rect() const noexcept;
	[[nodiscard]] const std::string& character_id() const noexcept;
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
	std::vector<EffectSpawnRequest> _pending_effect_requests;
	std::unique_ptr<Animation> _animation;
	AnimationState _animation_state = AnimationState::Idle;
	FacingDirection _facing_direction = FacingDirection::Right;
	Vector2 _move_input = Vector2::zero();
	Vector2 _desired_velocity = Vector2::zero();

	float _move_speed = 240.0f;
	float _hp = 100;
	float _mana = 100;

	Rect _collision_rect;

	bool _is_dead = false;
};
