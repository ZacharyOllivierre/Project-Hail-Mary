#include "startup_loading_scene.h"

#include "../../engine/io/path_manager.h"
#include "../../engine/resources/resource_manager.h"
#include "../../engine/scene/scene_manager.h"
#include "../../engine/input/input_state.h"
#include "menu_scene.h"

#include <SDL_image.h>
#include <iostream>

namespace
{
constexpr int kLogicalWidth = 1280;
constexpr int kLogicalHeight = 720;
constexpr int kSplashMaxWidth = 320;
constexpr int kSplashMaxHeight = 180;
constexpr int kSplashMargin = 24;

SDL_Rect build_bottom_right_rect(SDL_Texture* texture)
{
	if (!texture)
		return SDL_Rect{ 0, 0, 0, 0 };

	int texture_width = 0;
	int texture_height = 0;
	if (SDL_QueryTexture(texture, nullptr, nullptr, &texture_width, &texture_height) != 0
		|| texture_width <= 0
		|| texture_height <= 0)
	{
		return SDL_Rect{ 0, 0, 0, 0 };
	}

	const float width_ratio =
		static_cast<float>(kSplashMaxWidth) / static_cast<float>(texture_width);
	const float height_ratio =
		static_cast<float>(kSplashMaxHeight) / static_cast<float>(texture_height);
	const float scale = std::min(1.0f, std::min(width_ratio, height_ratio));

	const int draw_width = static_cast<int>(texture_width * scale);
	const int draw_height = static_cast<int>(texture_height * scale);

	return SDL_Rect{
		kLogicalWidth - draw_width - kSplashMargin,
		kLogicalHeight - draw_height - kSplashMargin,
		draw_width,
		draw_height
	};
}
}

void StartUpLoadingScene::on_enter()
{
	_paused = false;
	_renderer = nullptr;
	_state = LoadingState::WaitingForPreloadTexture;
	_error_message.clear();
}

void StartUpLoadingScene::on_update(double delta)
{
	Scene::on_update(delta);

	if (_state != LoadingState::ReadyToStartFullLoad || !_renderer)
		return;

	_state = LoadingState::Loading;

	if (!ResourceManager::instance()->init(_renderer))
	{
		_state = LoadingState::Failed;
		_error_message = "ResourceManager init fail";
		std::cout << "Startup loading failed: " << _error_message << std::endl;
		return;
	}

	SceneManager::instance()->switch_to<MenuScene>();
}

void StartUpLoadingScene::on_render(SDL_Renderer* renderer)
{
	Scene::on_render(renderer);

	if (!renderer)
		return;

	_renderer = renderer;

	if (_state == LoadingState::WaitingForPreloadTexture
		&& !ensure_preload_texture(renderer))
	{
		_state = LoadingState::Failed;
		return;
	}

	Uint8 old_r = 0;
	Uint8 old_g = 0;
	Uint8 old_b = 0;
	Uint8 old_a = 0;
	SDL_GetRenderDrawColor(renderer, &old_r, &old_g, &old_b, &old_a);

	SDL_SetRenderDrawColor(renderer, 12, 14, 20, 255);
	SDL_Rect background{ 0, 0, kLogicalWidth, kLogicalHeight };
	SDL_RenderFillRect(renderer, &background);

	render_preload_texture(renderer);

	if (_state == LoadingState::Failed)
	{
		SDL_SetRenderDrawColor(renderer, 180, 40, 40, 255);
		SDL_Rect fail_bar{ 0, kLogicalHeight - 8, kLogicalWidth, 8 };
		SDL_RenderFillRect(renderer, &fail_bar);
	}

	SDL_SetRenderDrawColor(renderer, old_r, old_g, old_b, old_a);
}

// Handles menu input, including keyboard input for starting the game.
void StartUpLoadingScene::on_input(
	const InputSnapshot& input,
	const std::vector<InputEvent>& events
)
{
	(void)events;

	Scene::on_input(input, events);
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

	if (!PathManager::instance()->init())
	{
		_error_message = "PathManager init fail";
		std::cout << "Startup preload failed: " << _error_message << std::endl;
		return false;
	}

	const std::filesystem::path preload_path =
		PathManager::instance()->resolve_asset_path("preload/loading.png");
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

	const SDL_Rect destination_rect = build_bottom_right_rect(_preload_texture);
	if (destination_rect.w <= 0 || destination_rect.h <= 0)
		return;

	SDL_RenderCopy(renderer, _preload_texture, nullptr, &destination_rect);
}

