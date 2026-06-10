#include "test_object.h"

#include "../engine/animation/animation_manager.h"
#include "../engine/core/render/render_command.h"
#include "../engine/input/input_state.h"

#include <iostream>
#include <utility>

TestObject::TestObject(std::string character_id)
	: GameObject(DepthLayer::Character),
	_character_id(std::move(character_id))
{
	set_position({ 200, 200 });
	set_size({ 100, 100 });
	set_animation_state(AnimationState::Idle);
	std::cout << "TestObject()" << std::endl;
}

TestObject::~TestObject()
{
	std::cout << "~TestObject()" << std::endl;
}

void TestObject::update(double delta)
{
	if (_animation)
		_animation->update(delta);

	if (!_is_dead && !_move_input.is_zero())
	{
		const Vector2 frame_move = _move_input.normalized() * (_move_speed * delta);
		set_position(position() + frame_move);
	}
}

void TestObject::submit_render_commands(std::vector<RenderCommand>& out_commands) const
{
	if (!_animation)
		return;

	RenderCommand command;
	if (_animation->build_render_command(world_rect(), 0.0, command))
	{
		command.flip = _facing_direction == FacingDirection::Left
			? SpriteFlip::Horizontal
			: SpriteFlip::None;
		out_commands.push_back(std::move(command));
	}
}

void TestObject::on_input_snapshot(const InputSnapshot& input)
{
	if (_is_dead)
		return;

	float move_x = 0.0f;
	float move_y = 0.0f;

	if (input.state.is_pressed(InputAction::Left))
		move_x -= 1.0f;
	if (input.state.is_pressed(InputAction::Right))
		move_x += 1.0f;
	if (input.state.is_pressed(InputAction::Up))
		move_y -= 1.0f;
	if (input.state.is_pressed(InputAction::Down))
		move_y += 1.0f;

	if (input.state.is_just_released(InputAction::DeleteKey))
	{
		die();
		return;
	}

	_move_input = { move_x, move_y };
	if (move_x < 0.0f)
		_facing_direction = FacingDirection::Left;
	else if (move_x > 0.0f)
		_facing_direction = FacingDirection::Right;

	if (_move_input.is_zero())
		set_animation_state(AnimationState::Idle);
	else
		set_animation_state(AnimationState::Move);
}

bool TestObject::is_dead() const noexcept
{
	return _is_dead;
}

void TestObject::set_animation_state(AnimationState new_state)
{
	if (_animation && _animation_state == new_state)
		return;

	const std::string animation_key = animation_key_for(new_state);
	std::unique_ptr<Animation> animation =
		AnimationManager::instance()->create_animation(animation_key);
	if (!animation)
	{
		std::cout << "Set TestObject animation failed: "
			<< animation_key << std::endl;
		return;
	}

	_animation = std::move(animation);
	_animation_state = new_state;
}

void TestObject::die()
{
	if (_is_dead)
		return;

	_is_dead = true;
	_move_input = Vector2::zero();
	set_animation_state(AnimationState::Die);
}

std::string TestObject::animation_key_for(AnimationState state) const
{
	switch (state)
	{
	case AnimationState::Move:
		return _character_id + ".move";

	case AnimationState::Die:
		return _character_id + ".die";

	case AnimationState::Idle:
	default:
		return _character_id + ".idle";
	}
}
