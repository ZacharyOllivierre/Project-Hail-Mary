#pragma once
#include "../game_object.h"
#include "../base/timer.h"

enum class FadeMode
{
    FadeIn,
    FadeOut,
    FadeInOut
};

class FadeImage : public GameObject
{
public:
    FadeImage(SDL_Texture* texture, Vector2 pos, Vector2 size);

    void set_play(FadeMode mode,double hold_time, float fade_in_duration, float fade_out_duration);
    void play();

    void on_update(double delta)override;
    void on_render(SDL_Renderer* renderer)override;

private:
    void update_fade_in(double delta);
    void update_fade_out(double delta);

    void start_hold();
    double ratio(double value, double max_value) const;

private:
    enum class FadeState
    {
        Idle,
        FadingIn,
        Holding,
        FadingOut,
        Finished
    };

private:
    FadeMode _mode = FadeMode::FadeIn;
    FadeState _state = FadeState::Idle;

    Timer _timer;

    SDL_Texture* _texture = nullptr;

    double _elapsed = 0.0;
    double _hold_time = 0.0;

    double _fade_in_duration = 1.0;
    double _fade_out_duration = 1.0;

    Uint8 _alpha = 255;
};
