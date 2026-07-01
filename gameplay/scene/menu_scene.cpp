#include "menu_scene.h"

#include "../../engine/scene/scene_manager.h"
#include "../../engine/resources/resource_manager.h"
#include "../../engine/input/input_state.h"

#include "game_scene.h"
#include "room_scene.h"

#include <iostream>
#include <SDL_ttf.h> // Added for text

// Menu text
void draw_text(
	SDL_Renderer *renderer,
	TTF_Font *font,
	const char *text,
	int x,
	int y,
	SDL_Color color)
{
	if (!renderer || !font || !text)
		return;

	SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);

	if (!surface)
	{
		std::cout << "Failed to create text surface: " << TTF_GetError() << std::endl;
		return;
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (!texture)
	{
		std::cout << "Failed to create text texture: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(surface);
		return;
	}

	SDL_Rect dst_rect{x, y, surface->w, surface->h};

	SDL_RenderCopy(renderer, texture, nullptr, &dst_rect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

// Runs when menu scene is active
void MenuScene::on_enter()
{
	_paused = false;

	std::cout << "Entered MenuScene" << std::endl;

	// Added for  menutext
	if (TTF_WasInit() == 0)
	{
		if (TTF_Init() == -1)
		{
			std::cout << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
			return;
		}
	}

	// The "assets/../..." Needs to ppoint to an actual font file
	_menu_font = ResourceManager::instance()->find_font("latin.16");

	if (!_menu_font)
	{
		std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
	}
	// Added for menu text
}

// Updates menu scene each frame.
void MenuScene::on_update(double delta)
{
	Scene::on_update(delta);
}

// Renders menu background, title area, and start button every frame.
void MenuScene::on_render(SDL_Renderer *renderer)
{
	Scene::on_render(renderer);

	// Stops rendering if the renderer is missing.
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
	SDL_Rect background{0, 0, 1280, 720};
	SDL_RenderFillRect(renderer, &background);

	// Simple title box
	SDL_SetRenderDrawColor(renderer, 80, 80, 140, 255);
	SDL_Rect title_rect{440, 120, 400, 80};
	SDL_RenderFillRect(renderer, &title_rect);

	// Draw menu text title
	SDL_Color white{255, 255, 255, 255};
	// Menu text
	draw_text(renderer, _menu_font, "MAIN MENU", 510, 140, white);

	// Creates start button only once if it does not already exist.
	if (!_start_button)
	{
		create_start_button(renderer);
	}

	// Renders start button if it exists.
	if (_start_button)
	{
		_start_button->render();
		// Menu text
		draw_text(renderer, _menu_font, "START", 590, 335, white);
	}

	// Restore old draw color
	SDL_SetRenderDrawColor(renderer, old_r, old_g, old_b, old_a);
}

// Handles menu input, including keyboard input for starting the game.
void MenuScene::on_input(
	const InputSnapshot &input,
	const std::vector<InputEvent> &events)
{
	(void)events;

	Scene::on_input(input, events);

	// Starts the gameplay scene if the confirm input, like Enter, was just pressed.
	if (input.state.is_just_pressed(InputAction::Confirm))
	{
		std::cout << "Start pressed from keyboard!" << std::endl;
		SceneManager::instance()->switch_to<GameScene>();
	}
}

// Runs when menu scene is exited.
void MenuScene::on_exit()
{
	std::cout << "Leaving MenuScene" << std::endl;

	// Destroy button when leaving the menu.
	_start_button.reset();

	// Related to menu text
	if (_menu_font)
	{
		TTF_CloseFont(_menu_font);
		_menu_font = nullptr;
	}

	_paused = false;
}

// Resets menu scene back to its starting state.
void MenuScene::reset()
{
	_start_button.reset();
	_paused = false;
}

// Sends raw SDL events, like mouse clicks, to the start button.
void MenuScene::handle_sdl_event(const SDL_Event &event)
{
	// Lets button handle the SDL event only if button exists.
	if (_start_button)
	{
		_start_button->handle_event(event);
	}
}

// Creates start button and connects it to the gameplay scene.
void MenuScene::create_start_button(SDL_Renderer *renderer)
{
	// Prevents creating another button if one already exists.
	if (_start_button)
		return;

	// x, y, width, height
	SDL_Rect button_rect{540, 320, 200, 70};

	_start_button = std::make_unique<Button>(renderer, button_rect);

	// Runs when start button is clicked.
	_start_button->set_on_click([]()
								{
		std::cout << "Start button clicked!" << std::endl;

		// This sends the program from menu_scene to game_scene.
		SceneManager::instance()->switch_to<RoomScene>(); });
}