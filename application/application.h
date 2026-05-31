#pragma once

#include <SDL.h>

class Application
{
public:
    static Application* instance();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    bool init(int argc, char** argv);
    int run(int argc, char** argv);
    void shutdown();

private:
    Application() = default;
    ~Application();

    void init_assert(bool flag, const char* err_msg);

    const int _logical_width = 1280;
    const int _logical_height = 720;

    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
    bool _initialized = false;
    bool _active = false;
};
