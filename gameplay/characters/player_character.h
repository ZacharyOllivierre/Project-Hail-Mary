#pragma once
#include "character.h"
#include "../../engine/input/contracts/input_snapshot_receiver.h"

class PlayerCharacter:public Character,
	public engine::input::InputSnapshotReceiver
{
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

public:
	void on_input_snapshot(const engine::input::InputSnapshot& input) override;

	// Wand
	std::vector<std::unique_ptr<Projectile>> create_projectile(const engine::core::Vector2& direction);

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

};