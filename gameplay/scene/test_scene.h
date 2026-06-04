
#pragma once

#include "../../engine/core/scene/scene.h"

class TestScene final : public Scene
{
public:
	TestScene() = default;
	~TestScene() override = default;

	void on_update()override;
	void on_render(SDL_Renderer* renderer)override;
	void on_input()override;

	void on_enter() override;
	void on_exit() override;
	void reset() override;
};
