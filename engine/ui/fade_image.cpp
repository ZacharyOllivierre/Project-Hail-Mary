#include "fade_image.h"

FadeImage::FadeImage(SDL_Texture* texture, Vector2 pos, Vector2 size)
	:GameObject(DepthLayer::UI),_texture(texture)
{
	GameObject::set_world_position(pos);
	GameObject::set_size(size);

    if (_texture)
        SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
}

void FadeImage::set_play(FadeMode mode, double hold_time, float fade_in_duration, float fade_out_duration)
{
	_mode = mode;

    if (hold_time < 0)
        hold_time = 0.0;

    _hold_time = hold_time;
	_timer.set_wait_time(_hold_time);
    _timer.set_on_timeout([this] {
        switch (_mode)
        {
        case FadeMode::FadeIn:
            _state = FadeState::Finished;
            break;
        case FadeMode::FadeOut:
        case FadeMode::FadeInOut:
            _elapsed = 0.0;
            _state = FadeState::FadingOut;
                break;
        default:
            break;
        }
        });

    _fade_in_duration = fade_in_duration;
    _fade_out_duration = fade_out_duration;
}

void FadeImage::play()
{
    _elapsed = 0.0;

    switch (_mode)
    {
    case FadeMode::FadeIn:
    case FadeMode::FadeInOut:
        _alpha = 0;
        _state = FadeState::FadingIn;
        break;

    case FadeMode::FadeOut:
        _alpha = 255;
        start_hold();
        break;

    default:
        break;
    }
}

void FadeImage::on_update(double delta)
{
    if (_state == FadeState::Idle)
        return;

    switch (_state)
    {
    case FadeState::FadingIn:
        update_fade_in(delta);
        break;

    case FadeState::Holding:
        _timer.update(delta);
        break;

    case FadeState::FadingOut:
        update_fade_out(delta);
        break;

    default:
        break;
    }

    if (_state == FadeState::Finished)
        GameObject::destroy();
}

void FadeImage::on_render(SDL_Renderer* renderer)
{
    if (!_texture || !renderer)
        return;

    SDL_SetTextureAlphaMod(_texture, _alpha);
    const SDL_Rect& dst_rect = rect();
    SDL_RenderCopy(renderer, _texture, nullptr, &dst_rect);
    SDL_SetTextureAlphaMod(_texture, 255);
}

void FadeImage::update_fade_in(double delta)
{
    _elapsed += delta;

    double t = ratio(_elapsed, _fade_in_duration);
    _alpha = static_cast<Uint8>(255.0 * t);

    if (t >= 1.0)
    {
        _elapsed = 0.0;
        _alpha = 255;
        start_hold();
    }
}

void FadeImage::update_fade_out(double delta)
{
    _elapsed += delta;

    double t = ratio(_elapsed, _fade_out_duration);
    _alpha = static_cast<Uint8>(255.0 * (1.0 - t));

    if (t >= 1.0)
    {
        _alpha = 0;
        _state = FadeState::Finished;
    }
}

double FadeImage::ratio(double value, double max_value) const
{
    if (max_value <= 0.0)
        return 1.0;

    double t = value / max_value;

    if (t < 0.0)
        return 0.0;

    if (t > 1.0)
        return 1.0;

    return t;
}

void FadeImage::start_hold()
{
    if (_hold_time <= 0.0)
    {
        switch (_mode)
        {
        case FadeMode::FadeIn:
            _state = FadeState::Finished;
            break;
        case FadeMode::FadeOut:
        case FadeMode::FadeInOut:
            _elapsed = 0.0;
            _state = FadeState::FadingOut;
            break;
        default:
            break;
        }
        return;
    }

    _state = FadeState::Holding;
    _timer.restart();
}
