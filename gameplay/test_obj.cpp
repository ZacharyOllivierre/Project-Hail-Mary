#include "test_obj.h"

#include <iostream>

// Problem
// This shared_ptr keeps TestObj alive even after Scene removes it.

TestObj::TestObj() :GameObject(DepthLayer::Item)
{
	_test = ResourceManager::instance()->find_texture("test");
	GameObject::set_position({ 200,200 });
	GameObject::set_size({ 100,100 });
	std::cout << "TestObj()" << std::endl;

}

TestObj::~TestObj()
{
	std::cout << "~TestObj()" << std::endl;
}

void TestObj::update(double delta)
{
	if (!_move_input.is_zero())
	{
		const Vector2 frame_move = _move_input.normalized() * (_move_speed * delta);
		GameObject::move_by(frame_move);

		std::cout << GameObject::position().x << " " << GameObject::position().y << std::endl;
	}
}

void  TestObj::submit_render_commands(std::vector<RenderCommand>& out_commands)const
{
	RenderCommand com;
	com.world_rect = GameObject::world_rect();
	com.texture = _test;

	out_commands.push_back(std::move(com));
}

void TestObj::on_input_snapshot(const InputSnapshot& input)
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
		GameObject::destroy();

	_move_input = { move_x, move_y };
}
