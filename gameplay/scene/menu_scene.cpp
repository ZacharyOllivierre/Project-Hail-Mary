#include "menu_scene.h"

#include "../../engine/scene/scene_manager.h"
#include "../../engine/input/input_state.h"

#include "test_scene.h"

#include <iostream>

void MenuScene::on_enter()
{
	_paused = false;

	std::cout << "Entered MenuScene" << std::endl;
}

void MenuScene::on_update(double delta)
{
	Scene::on_update(delta);
}

void MenuScene::on_render(SDL_Renderer* renderer)
{
	Scene::on_render(renderer);

	if (!renderer)
		return;

	// Save old draw color
	Uint8 old_r = 0;
	Uint8 old_g = 0;
	Uint8 old_b = 0;
	Uint8 old_a = 0;
	SDL_GetRenderDrawColor(renderer, &old_r, &old_g, &old_b, &old_a);

	// Simple menu background
	SDL_SetRenderDrawColor(renderer, 20, 20, 35, 255);
	SDL_Rect background{ 0, 0, 1280, 720 };
	SDL_RenderFillRect(renderer, &background);

	// Simple title box
	SDL_SetRenderDrawColor(renderer, 80, 80, 140, 255);
	SDL_Rect title_rect{ 440, 120, 400, 80 };
	SDL_RenderFillRect(renderer, &title_rect);

	// Create button once.
	// This is done here because Button needs the SDL_Renderer.
	if (!_start_button)
	{
		create_start_button(renderer);
	}

	// Draw button every frame.
	if (_start_button)
	{
		_start_button->render();
	}

	// Restore old draw color
	SDL_SetRenderDrawColor(renderer, old_r, old_g, old_b, old_a);
}

void MenuScene::on_input(
	const InputSnapshot& input,
	const std::vector<InputEvent>& events
)
{
	(void)events;

	Scene::on_input(input, events);

	// Optional: pressing Enter also starts the game.
	if (input.state.is_just_pressed(InputAction::Confirm))
	{
		std::cout << "Start pressed from keyboard!" << std::endl;
		SceneManager::instance()->switch_to<TestScene>();
	}
}

void MenuScene::on_exit()
{
	std::cout << "Leaving MenuScene" << std::endl;

	// Destroy button when leaving the menu.
	_start_button.reset();

	_paused = false;
}

void MenuScene::reset()
{
	_start_button.reset();
	_paused = false;
}

void MenuScene::handle_sdl_event(const SDL_Event& event)
{
	if (_start_button)
	{
		_start_button->handle_event(event);
	}
}

void MenuScene::create_start_button(SDL_Renderer* renderer)
{
	if (_start_button)
		return;

	// x, y, width, height
	SDL_Rect button_rect{ 540, 320, 200, 70 };

	_start_button = std::make_unique<Button>(renderer, button_rect);

	_start_button->set_on_click([]()
	{
		std::cout << "Start button clicked!" << std::endl;

		// This sends the program from menu_scene to test_scene.
		SceneManager::instance()->switch_to<TestScene>();
	});
}