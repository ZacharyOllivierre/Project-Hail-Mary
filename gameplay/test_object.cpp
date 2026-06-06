#include "test_object.h"

#include "../engine/core/render/render_command.h"
#include "../engine/input/input_state.h"

#include <iostream>

TestObject::TestObject() : GameObject(DepthLayer::Item)
{
	_texture = ResourceManager::instance()->find_texture("test");
	set_position({ 200, 200 });
	set_size({ 100, 100 });
	std::cout << "TestObject()" << std::endl;

}

TestObject::~TestObject()
{
	std::cout << "~TestObject()" << std::endl;
}

void TestObject::update(double delta)
{
	if (!_move_input.is_zero())
	{
		const Vector2 frame_move = _move_input.normalized() * (_move_speed * delta);
		set_position(position() + frame_move);
	}
}

void TestObject::submit_render_commands(std::vector<RenderCommand>& out_commands) const
{
	RenderCommand command;
	command.command_rect = world_rect();
	command.texture = _texture;

	out_commands.push_back(std::move(command));
}

void TestObject::on_input_snapshot(const InputSnapshot& input)
{
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
		destroy();

	_move_input = { move_x, move_y };
}
