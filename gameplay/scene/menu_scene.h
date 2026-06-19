#pragma once

#include "../../engine/scene/scene.h"
#include "../../engine/ui/button.h"

#include <SDL.h>
#include <SDL_ttf.h> // Text
#include <memory>
#include <vector>

class MenuScene final : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() override = default;

	void on_enter() override;
	void on_update(double delta) override;
	void on_render(SDL_Renderer* renderer) override;
	void on_input(const InputSnapshot& input, const std::vector<InputEvent>& events) override;
	void on_exit() override;
	void reset() override;

	// Lets Application send raw SDL mouse events to the menu button.
	void handle_sdl_event(const SDL_Event& event);

private:
	void create_start_button(SDL_Renderer* renderer);

	std::unique_ptr<Button> _start_button;

	TTF_Font* _menu_font = nullptr; // Added for text
};