#include "application.h"

#include "../engine/audio/audio_service.h"
#include "../engine/io/path_manager.h"
#include "../engine/scene/scene_manager.h"

#include "../gameplay/scene/menu_scene.h"
#include "../gameplay/scene/startup_loading_scene.h"
#include "../thirdparty/imgui/imgui.h"
#include "../thirdparty/imgui/imgui_impl_sdl2.h"
#include "../thirdparty/imgui/imgui_impl_sdlrenderer2.h"

#include <ctime>
#include <iostream>

#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

Application::Application()
{
	init_assert(!SDL_Init(SDL_INIT_EVERYTHING), "SDL2 Error");

	int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;
	init_assert((IMG_Init(img_flags) & img_flags) == img_flags, "SDL_image Error");

	int mix_flags = MIX_INIT_MP3;
	init_assert((Mix_Init(mix_flags) & mix_flags) == mix_flags, "SDL_mixer Error");

	init_assert(!TTF_Init(), "SDL_ttf Error");
	init_assert(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == 0, "Mix_OpenAudio Error");

	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

	_window = SDL_CreateWindow("window_name", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _logical_width, _logical_height, SDL_WINDOW_SHOWN);
	init_assert(_window, "SDL_CreateWindow Error");

	if (SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
	{
		SDL_Log("Failed to enter fullscreen: %s", SDL_GetError());
		SDL_ClearError();

		SDL_SetWindowSize(_window, _logical_width, _logical_height);
		SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	init_assert(_renderer, "SDL_CreateRenderer Error");

	init_assert(SDL_RenderSetLogicalSize(_renderer, _logical_width, _logical_height) == 0, "SDL_RenderSetLogicalSize Error");

	//imgui debug
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	init_assert(
		ImGui_ImplSDL2_InitForSDLRenderer(_window, _renderer)
		&& ImGui_ImplSDLRenderer2_Init(_renderer),
		"ImGui init error");
}

Application::~Application()
{
	//imgui debug
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);

	TTF_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Application::init(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	init_assert(engine::io::PathManager::instance()->init(), "engine::io::PathManager init fail");
	init_assert(engine::io::PathManager::instance()->ensure_runtime_dirs(), "Runtime dir init fail");
	init_assert(
		engine::audio::AudioService::instance()->init({}),
		"AudioService init fail");

	return true;
}
int Application::run(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	Uint64 last_counter = SDL_GetPerformanceCounter();
	const Uint64 counter_freq = SDL_GetPerformanceFrequency();
	std::srand(static_cast<unsigned>(std::time(nullptr)));

	_counter_freq = SDL_GetPerformanceFrequency();
	_last_counter = SDL_GetPerformanceCounter();

	engine::scene::SceneManager::instance()->switch_to<StartUpLoadingScene>();

	while (_active)
	{
		_input_system.begin_frame();
		while (SDL_PollEvent(&_event))
		{
			if (_event.type == SDL_QUIT)
				_active = false;
			//imgui debug
			ImGui_ImplSDL2_ProcessEvent(&_event);
			_input_system.process_event(_event);

			// Added for main menu
			// After SDL recieves an event and processes it, check if a menu exists
			if (MenuScene* menu_scene = engine::scene::SceneManager::instance()->try_find_scene<MenuScene>())
			{
				// Forward raw SDL event to scene, allows button to react to mouse clicks and SDL events
				menu_scene->handle_sdl_event(_event);
			}

		}

		engine::scene::SceneManager::instance()->on_input(
			_input_system.snapshot(),
			_input_system.events());

		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta = static_cast<double>(current_counter - last_counter) / counter_freq;
		last_counter = current_counter;

		

		engine::scene::SceneManager::instance()->on_update(delta);
		engine::audio::AudioService::instance()->update(delta);

		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
		SDL_RenderClear(_renderer);

		//imgui debug
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();

		ImGuiIO& imgui_io = ImGui::GetIO();
		imgui_io.DisplaySize = ImVec2(
			static_cast<float>(_logical_width),
			static_cast<float>(_logical_height));

		ImGui::NewFrame();

		engine::scene::SceneManager::instance()->on_render(_renderer);
		//imgui debug
		engine::scene::SceneManager::instance()->on_imgui();
		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), _renderer);

		SDL_RenderPresent(_renderer);
	}

	shutdown();

	return 0;
}

void Application::shutdown()
{
	engine::scene::SceneManager::instance()->shutdown();
	engine::audio::AudioService::instance()->shutdown();
}
