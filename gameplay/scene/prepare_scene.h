#pragma once

#include "../../engine/scene/scene.h"

class PrepareScene : public engine::scene::Scene
{
	PrepareScene();
	~PrepareScene();

	void on_enter() override;
	void on_update(double delta) override;
	void on_render(SDL_Renderer* renderer) override;
	void on_input(const engine::input::InputSnapshot& input, const std::vector<engine::input::InputEvent>& events) override;
	void on_exit() override;
	void reset() override;
};