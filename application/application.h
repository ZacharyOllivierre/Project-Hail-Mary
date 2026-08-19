#pragma once
#include "../engine/tools/singleton.h"
#include "../engine/tools/logger.h"
#include "../engine/input/input_system.h"

#include <SDL.h>
#include <cstdlib>
#include <source_location>

class Application: public engine::tools::Singleton<Application>
{
    friend engine::tools::Singleton<Application>;
public:
    Application();
    ~Application();

    bool init(int argc, char** argv);
    int run(int argc, char** argv);
    void shutdown();


    void init_assert(bool flag, const char* err_msg,
        std::source_location location = std::source_location::current())
    {
        if (flag)
            return;
        auto* logger = engine::tools::Logger::instance();
        logger->error("application",err_msg,location);
        logger->terminating("application","Application startup aborted.",location);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Game Start Error", err_msg, _window);
        exit(-1);
    }

private:

    const int _logical_width = 1280;
    const int _logical_height = 720;

    Uint64 _last_counter = 0;
    Uint64 _counter_freq = 0;

    SDL_Event _event;
    engine::input::InputSystem _input_system;

    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;

    bool _active = { true };

};
