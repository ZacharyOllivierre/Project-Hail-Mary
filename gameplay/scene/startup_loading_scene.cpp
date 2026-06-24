//#include "startup_loading_scene.h"
//
//#include "../../engine/scene/scene_manager.h"
//#include "../../engine/input/input_state.h"
//
//#include "game_scene.h"
//
//#include <iostream>
//
//void StartUpLoadingScene::on_enter()
//{
//
//}
//
//void StartUpLoadingScene::on_update(double delta)
//{
//	Scene::on_update(delta);
//}
//
//void MenuScene::on_render(SDL_Renderer* renderer)
//{
//	Scene::on_render(renderer);
//
//	if (!renderer)
//		return;
//
//	Uint8 old_r = 0;
//	Uint8 old_g = 0;
//	Uint8 old_b = 0;
//	Uint8 old_a = 0;
//	SDL_GetRenderDrawColor(renderer, &old_r, &old_g, &old_b, &old_a);
//
//	SDL_SetRenderDrawColor(renderer, 20, 20, 35, 255);
//	SDL_Rect background{ 0, 0, 1280, 720 };
//	SDL_RenderFillRect(renderer, &background);
//
//	SDL_SetRenderDrawColor(renderer, 80, 80, 140, 255);
//	SDL_Rect title_rect{ 440, 120, 400, 80 };
//	SDL_RenderFillRect(renderer, &title_rect);
//
//	// Creates start button only once if it does not already exist.
//	if (!_start_button)
//	{
//		create_start_button(renderer);
//	}
//
//	// Renders start button if it exists.
//	if (_start_button)
//	{
//		_start_button->render();
//	}
//
//	// Restore old draw color
//	SDL_SetRenderDrawColor(renderer, old_r, old_g, old_b, old_a);
//}
//
//// Handles menu input, including keyboard input for starting the game.
//void MenuScene::on_input(
//	const InputSnapshot& input,
//	const std::vector<InputEvent>& events
//)
//{
//	(void)events;
//
//	Scene::on_input(input, events);
//
//	// Starts the gameplay scene if the confirm input, like Enter, was just pressed.
//	if (input.state.is_just_pressed(InputAction::Confirm))
//	{
//		std::cout << "Start pressed from keyboard!" << std::endl;
//		SceneManager::instance()->switch_to<GameScene>();
//	}
//}
//
//// Runs when menu scene is exited.
//void MenuScene::on_exit()
//{
//	std::cout << "Leaving MenuScene" << std::endl;
//
//	// Destroy button when leaving the menu.
//	_start_button.reset();
//
//	_paused = false;
//}
//
//// Resets menu scene back to its starting state.
//void MenuScene::reset()
//{
//	_start_button.reset();
//	_paused = false;
//}
//
//// Sends raw SDL events, like mouse clicks, to the start button.
//void MenuScene::handle_sdl_event(const SDL_Event& event)
//{
//	// Lets button handle the SDL event only if button exists.
//	if (_start_button)
//	{
//		_start_button->handle_event(event);
//	}
//}
//
//// Creates start button and connects it to the gameplay scene.
//void MenuScene::create_start_button(SDL_Renderer* renderer)
//{
//	// Prevents creating another button if one already exists.
//	if (_start_button)
//		return;
//
//	// x, y, width, height
//	SDL_Rect button_rect{ 540, 320, 200, 70 };
//
//	_start_button = std::make_unique<Button>(renderer, button_rect);
//
//	// Runs when start button is clicked.
//	_start_button->set_on_click([]()
//	{
//		std::cout << "Start button clicked!" << std::endl;
//
//		// This sends the program from menu_scene to game_scene.
//		SceneManager::instance()->switch_to<GameScene>();
//	});
//}
