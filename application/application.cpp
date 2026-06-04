#include "application.h"

#include "../engine/core/scene/scene_manager.h"
#include "../engine/resources/resource_manager.h"

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
}

Application::~Application()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);

	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Application::init(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	ResourceManager::instance()->init(_renderer);

	return true;
}

int Application::run(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	Uint64 last_counter = SDL_GetPerformanceCounter();
	const Uint64 counter_freq = SDL_GetPerformanceFrequency();
	std::srand(static_cast<unsigned>(std::time(nullptr)));

	_counter_freq = SDL_GetPerformanceFrequency();
	_last_counter = SDL_GetPerformanceCounter();

	while (_active)
	{
		_input_system.begin_frame();
		while (SDL_PollEvent(&_event))
		{
			if (_event.type == SDL_QUIT)
				_active = false;
			_input_system.process_event(_event);
		}

		SceneManager::instance()->on_input(
			_input_system.snapshot(),
			_input_system.events()
		);

		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta = static_cast<double>(current_counter - last_counter) / counter_freq;
		last_counter = current_counter;

		if (delta * 1000 < 1000.0 / FPS)
			SDL_Delay(static_cast<Uint32>(1000.0 / FPS - delta * 1000));

		SceneManager::instance()->on_update(delta);

		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
		SDL_RenderClear(_renderer);

		SceneManager::instance()->on_render(_renderer);

		//---------------test--------------------
		SDL_Rect dst{ 100,100,100,100 };
		SDL_RenderCopy(_renderer, ResourceManager::instance()->find_texture("test"), nullptr, &dst);
		//---------------test--------------------

		SDL_RenderPresent(_renderer);
	}

	shutdown();

	return 0;
}

void Application::shutdown()
{
	//...
}
