#pragma once
#include <SDL.h>
#include "../game_object.h"

class ProgressBar : public GameObject
{
public:
    ProgressBar(Vector2 position, Vector2 size);
    ~ProgressBar()=default;

    void on_render(SDL_Renderer* renderer)override;
    void reset()override;
    void set_progress(float value);

private:
    float _progress = 0.0f;

    SDL_Color _bg_color{ 0, 43, 100, 255 };
    SDL_Color _fill_color{ 245, 255, 255, 255 };
};
