#pragma once

#include <memory>
#include <string>

#include "../engine/animation/animation.h"
#include "../engine/core/game_object.h"
#include "../engine/core/interface/updatable.h"
#include "../engine/input/contracts/input_snapshot_receiver.h"

class TestObject : public GameObject, public Updatable, public InputSnapshotReceiver
{
public:
	explicit TestObject(std::string character_id = "joker");
	~TestObject() override;

	void update(double delta) override;
	void on_input_snapshot(const InputSnapshot& input) override;

	void submit_render_commands(std::vector<RenderCommand>& out_commands) const override;
	bool is_dead() const noexcept;

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

	void set_animation_state(AnimationState new_state);
	void die();
	std::string animation_key_for(AnimationState state) const;

private:
	std::string _character_id;
	std::unique_ptr<Animation> _animation;
	AnimationState _animation_state = AnimationState::Idle;
	FacingDirection _facing_direction = FacingDirection::Right;
	bool _is_dead = false;
	Vector2 _move_input = Vector2::zero();
	float _move_speed = 240.0f;
};

