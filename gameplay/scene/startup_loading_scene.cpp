#include "startup_loading_scene.h"

#include "../../engine/io/path_manager.h"
#include "../../engine/resources/resource_manager.h"
#include "../../engine/scene/scene_manager.h"
#include "../../engine/input/input_state.h"
#include "menu_scene.h"

#include <SDL_image.h>
#include <iostream>

void StartUpLoadingScene::on_enter()
{
	_paused = false;
	_renderer = nullptr;
	_state = LoadingState::WaitingForPreloadTexture;
	_error_message.clear();
}

void StartUpLoadingScene::on_update(double delta)
{
	::engine::scene::Scene::on_update(delta);

	if (_state != LoadingState::ReadyToStartFullLoad || !_renderer)
		return;

	_state = LoadingState::Loading;

	if (!::engine::resources::ResourceManager::instance()->init(_renderer))
	{
		_state = LoadingState::Failed;
		_error_message = "::engine::resources::ResourceManager init fail";
		std::cout << "Startup loading failed: " << _error_message << std::endl;
		return;
	}

	::engine::scene::SceneManager::instance()->switch_to<MenuScene>();
}

void StartUpLoadingScene::on_render(SDL_Renderer* renderer)
{
	::engine::scene::Scene::on_render(renderer);

	if (!renderer)
		return;

	_renderer = renderer;

	if (_state == LoadingState::WaitingForPreloadTexture
		&& !ensure_preload_texture(renderer))
	{
		_state = LoadingState::Failed;
		return;
	}


	render_preload_texture(renderer);
}

// Handles menu input, including keyboard input for starting the game.
void StartUpLoadingScene::on_input(
	const ::engine::input::InputSnapshot& input,
	const std::vector<::engine::input::InputEvent>& events
)
{
	(void)events;

	::engine::scene::Scene::on_input(input, events);
}

// Runs when menu scene is exited.
void StartUpLoadingScene::on_exit()
{
	release_preload_texture();
	_renderer = nullptr;
	_paused = false;
}

// Resets menu scene back to its starting state.
void StartUpLoadingScene::reset()
{
	release_preload_texture();
	_renderer = nullptr;
	_state = LoadingState::WaitingForPreloadTexture;
	_error_message.clear();
	_paused = false;
}

void StartUpLoadingScene::release_preload_texture()
{
	if (_preload_texture)
	{
		SDL_DestroyTexture(_preload_texture);
		_preload_texture = nullptr;
	}
}

bool StartUpLoadingScene::ensure_preload_texture(SDL_Renderer* renderer)
{
	if (_preload_texture)
	{
		_state = LoadingState::ReadyToStartFullLoad;
		return true;
	}

	if (!::engine::io::PathManager::instance()->init())
	{
		_error_message = "::engine::io::PathManager init fail";
		std::cout << "Startup preload failed: " << _error_message << std::endl;
		return false;
	}

	const std::filesystem::path preload_path =
		::engine::io::PathManager::instance()->resolve_asset_path("preload/loading.png");
	_preload_texture = IMG_LoadTexture(renderer, preload_path.string().c_str());
	if (!_preload_texture)
	{
		_error_message = "Failed to load preload image: " + preload_path.string();
		std::cout << "Startup preload failed: " << _error_message
			<< " error: " << IMG_GetError() << std::endl;
		return false;
	}

	_state = LoadingState::ReadyToStartFullLoad;
	return true;
}

void StartUpLoadingScene::render_preload_texture(SDL_Renderer* renderer) const
{
	if (!_preload_texture || !renderer)
		return;

	const SDL_Rect destination_rect{ 886, 606, 400, 100 };
	SDL_RenderCopy(renderer, _preload_texture, nullptr, &destination_rect);
}

