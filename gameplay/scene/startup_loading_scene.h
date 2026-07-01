#pragma once

#include "../../engine/scene/scene.h"
#include "../../engine/ui/button.h"

#include <SDL.h>
#include <memory>
#include <vector>

class StartUpLoadingScene final : public Scene
{
public:
	StartUpLoadingScene() = default;
	~StartUpLoadingScene() override = default;

	void on_enter() override;
	void on_update(double delta) override;
	void on_render(SDL_Renderer* renderer) override;
	void on_input(const InputSnapshot& input, const std::vector<InputEvent>& events) override;
	void on_exit() override;
	void reset() override;

private:

};