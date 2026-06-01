#include "application.h"

#include "../engine/ui/progress_bar.h"//test
#include "../engine/ui/fade_image.h"//test

#include "../engine/scene/scene_manager.h"


#include <iostream>
#include <thread>
#include <atomic>
#include <filesystem>

#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>


Application:: Application()
{
	init_assert(!SDL_Init(SDL_INIT_EVERYTHING), "SDL2 Error");

	int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;
	init_assert((IMG_Init(img_flags) & img_flags) == img_flags, "SDL_image Error");

	int mix_flags = MIX_INIT_MP3;
	init_assert((Mix_Init(mix_flags) & mix_flags) == mix_flags, "SDL_mixer Error");

	init_assert(!TTF_Init(), "SDL_ttf Error");
	init_assert(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == 0, "Mix_OpenAudio Error");

	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

	_window = SDL_CreateWindow("Moonline", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _logical_width, _logical_height, SDL_WINDOW_SHOWN);
	init_assert(_window, "SDL_CreateWindow Error");

	if (SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
	{
		SDL_Log("Failed to enter fullscreen: %s", SDL_GetError());
		SDL_ClearError();

		//fallback
		SDL_SetWindowSize(_window, _logical_width, _logical_height);
		SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);//硬件加速 垂直同步 目标纹理
	init_assert(_renderer, "SDL_CreateRenderer Error");

	init_assert(SDL_RenderSetLogicalSize(_renderer, _logical_width, _logical_height) == 0, "SDL_RenderSetLogicalSize Error");
}

Application:: ~Application()
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
	FadeImage image(preload_texture_result._texture.get(), { 530,270 }, { 250,250 });

	ProgressBar load_bar({ 1000.0f, 680.0f }, { 200.0f, 5.0f });

	image.set_play(FadeMode::FadeInOut, 2, 1, 1);
	image.play();


	std::atomic<int> progress_count = 0;
	std::atomic<bool> loading_finished = false;

	auto loading_function = [&]()
		{
			for (int i = 1; i <= 20; ++i)
			{
				//std::cout << "loading...\n";

				std::this_thread::sleep_for(std::chrono::milliseconds(500));

				progress_count.store(i*5, std::memory_order_relaxed);
			}

			loading_finished.store(true, std::memory_order_release);
		};

	std::thread loading_thread(loading_function);

	//
	Uint64 last_counter = SDL_GetPerformanceCounter();
	const Uint64 counter_freq = SDL_GetPerformanceFrequency();
	std::srand(static_cast<unsigned>(std::time(nullptr)));

	while (!loading_finished.load(std::memory_order_acquire))
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				loading_finished.store(true, std::memory_order_release);
			}
		}

		Uint64 current_counter = SDL_GetPerformanceCounter();//实现动态延时
		double delta = (double)(current_counter - last_counter) / counter_freq;
		last_counter = current_counter;

		if (delta * 1000 < 1000.0 / FPS)
			SDL_Delay((Uint32)(1000.0 / FPS - delta * 1000));

		image.on_update(delta);

		float progress = progress_count.load(std::memory_order_relaxed) / 100.0f;
		load_bar.set_progress(progress);

		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
		SDL_RenderClear(_renderer);

		//test
		load_bar.on_render(_renderer);
		image.on_render(_renderer);

		SDL_RenderPresent(_renderer);
	}

	std::cout << "good" << std::endl;
	//----------------------testing------------------------

	if (loading_thread.joinable())
		loading_thread.join();

    return true;
}

int  Application::run(int argc, char** argv)
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
		double delta = (double)(current_counter - last_counter) / counter_freq;
		last_counter = current_counter;

		if (delta * 1000 < 1000.0 / FPS)
			SDL_Delay((Uint32)(1000.0 / FPS - delta * 1000));
		

		SceneManager::instance()->on_update(delta);

		SDL_SetRenderDrawColor(_renderer, 0,0,0,0);
		SDL_RenderClear(_renderer);

		SceneManager::instance()->on_render(_renderer);

		SDL_RenderPresent(_renderer);
	}

	shutdown();

    return 0;
}

void Application::shutdown()
{
	//...
}


