#include "application.h"

#include <cstdlib>

Application* Application::instance()
{
    static Application app;
    return &app;
}

Application::~Application()
{
    shutdown();
}

void Application::init_assert(bool flag, const char* err_msg)
{
    if (flag)
    {
        return;
    }

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL2 Error", err_msg, _window);
    std::exit(EXIT_FAILURE);
}

bool Application::init(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    if (_initialized)
    {
        return true;
    }

    init_assert(SDL_Init(SDL_INIT_VIDEO) == 0, SDL_GetError());

    _window = SDL_CreateWindow(
        "SDL2 Minimal Template",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        _logical_width,
        _logical_height,
        SDL_WINDOW_SHOWN
    );
    init_assert(_window != nullptr, SDL_GetError());

    _renderer = SDL_CreateRenderer(
        _window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    init_assert(_renderer != nullptr, SDL_GetError());

    _initialized = true;
    _active = true;
    return true;
}

int Application::run(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    if (!_initialized && !init(argc, argv))
    {
        return -1;
    }

    SDL_Event event{};
    while (_active)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                _active = false;
            }
        }

        SDL_SetRenderDrawColor(_renderer, 24, 28, 36, 255);
        SDL_RenderClear(_renderer);

        SDL_Rect rect{
            ( _logical_width - 320) / 2,
            (_logical_height - 180) / 2,
            320,
            180
        };
        SDL_SetRenderDrawColor(_renderer, 99, 179, 237, 255);
        SDL_RenderFillRect(_renderer, &rect);

        SDL_RenderPresent(_renderer);
    }

    shutdown();
    return 0;
}

void Application::shutdown()
{
    if (_renderer != nullptr)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }

    if (_window != nullptr)
    {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }

    if (_initialized)
    {
        SDL_Quit();
        _initialized = false;
    }

    _active = false;
}
