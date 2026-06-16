#pragma once

#include "../../engine/scene/scene.h"
#include "../character.h"

class GameScene final : public Scene
{
public:
	GameScene() = default;
	~GameScene() override = default;

	void on_update(double delta) override;
	void on_render(SDL_Renderer* renderer) override;
	void on_input(const InputSnapshot& input, const std::vector<InputEvent>& events) override;

	void on_enter() override;
	void on_exit() override;
	void reset() override;

private:

};
