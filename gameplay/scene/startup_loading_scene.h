#pragma once

#include "../../engine/scene/scene.h"

#include <SDL.h>
#include <string>

class StartUpLoadingScene final : public engine::scene::Scene
{
public:
	StartUpLoadingScene() = default;
	~StartUpLoadingScene() override = default;

	void on_enter() override;
	void on_update(double delta) override;
	void on_render(SDL_Renderer* renderer) override;
	void on_input(const engine::input::InputSnapshot& input, const std::vector<engine::input::InputEvent>& events) override;
	void on_exit() override;
	void reset() override;

private:
	enum class LoadingState
	{
		WaitingForPreloadTexture,
		ReadyToStartFullLoad,
		Loading,
		Failed
	};

	void release_preload_texture();
	bool ensure_preload_texture(SDL_Renderer* renderer);
	void render_preload_texture(SDL_Renderer* renderer) const;

private:
	SDL_Texture* _preload_texture = nullptr;
	SDL_Renderer* _renderer = nullptr;
	LoadingState _state = LoadingState::WaitingForPreloadTexture;
	std::string _error_message;
};
