#include "button.h"

#include <stdexcept>
#include <utility>

Button::Button(SDL_Renderer* renderer, SDL_Rect rect_button)
{
    init_assert(renderer, "Button renderer must not be null.");
    this->renderer = renderer;
    this->rect_button = rect_button;
}

Button::Button(SDL_Renderer* renderer, SDL_Rect rect_button, SDL_Rect rect_message, SDL_Texture* texture_message,
    Mix_Chunk* sound_effect_down, Mix_Chunk* sound_effect_up)
    : Button(renderer, rect_button)
{
    this->rect_message = rect_message;
    this->texture_message = texture_message;
    this->sound_effect_down = sound_effect_down;
    this->sound_effect_up = sound_effect_up;
}

Button::Button(SDL_Renderer* renderer, SDL_Rect rect_button, SDL_Rect rect_message, SDL_Texture* texture_message,
    Mix_Chunk* sound_effect_down, Mix_Chunk* sound_effect_up,
    SDL_Color color_idle, SDL_Color color_hovered, SDL_Color color_pushed, SDL_Color color_frame)
    : Button(renderer, rect_button, rect_message, texture_message, sound_effect_down, sound_effect_up)
{
    this->color_idle = color_idle;
    this->color_hovered = color_hovered;
    this->color_pushed = color_pushed;
    this->color_frame = color_frame;
}

Button::Button(SDL_Renderer* renderer, SDL_Rect rect_button, SDL_Rect rect_message, SDL_Texture* texture_message,
    Mix_Chunk* sound_effect_down, Mix_Chunk* sound_effect_up,
    SDL_Texture* texture_idle, SDL_Texture* texture_hovered, SDL_Texture* texture_pushed)
    : Button(renderer, rect_button, rect_message, texture_message, sound_effect_down, sound_effect_up)
{
    init_assert(texture_idle, "Button texture_idle must not be null.");
    init_assert(texture_hovered, "Button texture_hovered must not be null.");
    init_assert(texture_pushed, "Button texture_pushed must not be null.");

    have_texture = true;
    this->texture_idle = texture_idle;
    this->texture_hovered = texture_hovered;
    this->texture_pushed = texture_pushed;
}

void Button::init_assert(const void* ptr, const char* err_msg)const
{
    if (ptr == nullptr)
    {
        throw std::invalid_argument(err_msg);
    }
}

void Button::render()
{
    if (!have_texture)
    {
        switch (status)
        {
        case Status::Idle:
            SDL_SetRenderDrawColor(renderer, color_idle.r, color_idle.g, color_idle.b, color_idle.a);
            break;
        case Status::Hovered:
            SDL_SetRenderDrawColor(renderer, color_hovered.r, color_hovered.g, color_hovered.b, color_hovered.a);
            break;
        case Status::Pushed:
            SDL_SetRenderDrawColor(renderer, color_pushed.r, color_pushed.g, color_pushed.b, color_pushed.a);
            break;
        }

        SDL_RenderFillRect(renderer, &rect_button);
        SDL_SetRenderDrawColor(renderer, color_frame.r, color_frame.g, color_frame.b, color_frame.a);
        SDL_RenderDrawRect(renderer, &rect_button);
    }
    else
    {
        SDL_Texture* current_texture = texture_idle;
        switch (status)
        {
        case Status::Idle:
            current_texture = texture_idle;
            break;
        case Status::Hovered:
            current_texture = texture_hovered;
            break;
        case Status::Pushed:
            current_texture = texture_pushed;
            break;
        }

        SDL_RenderCopy(renderer, current_texture, nullptr, &rect_button);
    }

    if (texture_message != nullptr)
    {
        SDL_RenderCopy(renderer, texture_message, nullptr, &rect_message);
    }
}

bool Button::handle_event(const SDL_Event& event)
{
    if (!enabled)
    {
        return false;
    }

    switch (event.type)
    {
    case SDL_MOUSEMOTION:
        if (is_pressing)
        {
            return true;
        }
        else
        {
            return update_hover_status(event.motion.x, event.motion.y);
        }

    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT && check_cursor_hit(event.button.x, event.button.y))
        {
            is_pressing = true;
            status = Status::Pushed;
            if (sound_effect_down != nullptr)
            {
                Mix_PlayChannel(-1, sound_effect_down, 0);
            }
            return true;
        }
        return false;

    case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_LEFT && is_pressing)
        {
            is_pressing = false;
            if (sound_effect_up != nullptr)
            {
                Mix_PlayChannel(-1, sound_effect_up, 0);
            }

            if (check_cursor_hit(event.button.x, event.button.y))
            {
                ++time_on_click;
                if (on_click)
                {
                    on_click();
                }
            }
            status = Status::Idle;
            return true;
        }
        return false;

    default:
        return false;
    }
}

bool Button::check_cursor_hit(int x, int y)const
{
    const SDL_Point cursor = { x, y };
    return SDL_PointInRect(&cursor, &rect_button) == SDL_TRUE;
}

bool Button::update_hover_status(int x, int y)
{
    const Status new_status = check_cursor_hit(x, y) ? Status::Hovered : Status::Idle;
    const bool changed = status != new_status;
    status = new_status;
    return changed || new_status == Status::Hovered;
}

void Button::set_on_click(std::function<void()> func)
{
    on_click = std::move(func);
}

Button::Status Button::get_status()const
{
    return status;
}

void Button::set_button_rect(SDL_Rect new_rect_button)
{
    rect_button = new_rect_button;
}

void Button::set_message_rect(SDL_Rect new_rect_message)
{
    rect_message = new_rect_message;
}

void Button::set_message_texture(SDL_Texture* new_texture_message)
{
    texture_message = new_texture_message;
}

void Button::set_enabled(bool new_enabled)
{
    enabled = new_enabled;
    if (!enabled)
    {
        is_pressing = false;
        status = Status::Idle;
    }
}

bool Button::is_enabled() const
{
    return enabled;
}

int Button::click_count() const
{
    return time_on_click;
}

void Button::reset_click_count()
{
    time_on_click = 0;
}


