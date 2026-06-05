#pragma once

#include "../../engine/scene/scene.h"

class TestScene final : public Scene
{
public:
	TestScene() = default;
	~TestScene() override = default;

	void on_update(double delta)override;
	void on_render(SDL_Renderer* renderer)override;
	void on_input(const InputSnapshot& input, const std::vector<InputEvent>& events)override;

	void on_enter() override;
	void on_exit() override;
	void reset() override;
};
