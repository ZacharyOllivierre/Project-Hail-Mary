#include "keyboard_mouse_input_translator.h"

#include <SDL.h>

namespace engine::input
{

std::vector<InputEvent> KeyboardMouseInputTranslator::translate_event(const SDL_Event& event) const
{
    std::vector<InputEvent> events;

    switch (event.type)
    {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
    {
        if (event.type == SDL_KEYDOWN && event.key.repeat != 0)
        {
            return events;
        }

        const std::optional<InputAction> action = action_from_key(event.key.keysym.sym);
        if (action)
        {
            append_event(
                events,
                *action,
                input_event_type(event.type == SDL_KEYDOWN),
                InputDevice::Keyboard
            );
        }

        break;
    }

    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    {
        const std::optional<InputAction> action = action_from_mouse_button(event.button.button);
        if (action)
        {
            append_event(
                events,
                *action,
                input_event_type(event.type == SDL_MOUSEBUTTONDOWN),
                InputDevice::Mouse
            );
        }

        break;
    }

    case SDL_MOUSEWHEEL:
    {
        InputEvent input_event;
        input_event.type = InputEventType::MouseWheel;
        input_event.device = InputDevice::Mouse;
        input_event.wheel_x = event.wheel.x;
        input_event.wheel_y = event.wheel.y;
        events.push_back(input_event);
        break;
    }

    case SDL_TEXTINPUT:
    {
        InputEvent input_event;
        input_event.type = InputEventType::TextInput;
        input_event.device = InputDevice::Keyboard;
        input_event.text = event.text.text;
        events.push_back(input_event);
        break;
    }

    case SDL_TEXTEDITING:
    {
        InputEvent input_event;
        input_event.type = InputEventType::TextEditing;
        input_event.device = InputDevice::Keyboard;
        input_event.text = event.edit.text;
        events.push_back(input_event);
        break;
    }

    default:
        break;
    }

    return events;
}

std::optional<InputAction> KeyboardMouseInputTranslator::action_from_key(SDL_Keycode key) const
{
    switch (key)
    {
    case SDLK_a:
    case SDLK_LEFT:
        return InputAction::Left;
    case SDLK_d:
    case SDLK_RIGHT:
        return InputAction::Right;
    case SDLK_w:
    case SDLK_UP:
        return InputAction::Up;
    case SDLK_s:
    case SDLK_DOWN:
        return InputAction::Down;
    case SDLK_RETURN:
    case SDLK_KP_ENTER:
        return InputAction::Confirm;
    case SDLK_ESCAPE:
        return InputAction::Cancel;
    case SDLK_p:
        return InputAction::Pause;
    case SDLK_j:
        return InputAction::Attack;
    case SDLK_k:
        return InputAction::Special;
    case SDLK_BACKSPACE:
        return InputAction::Backspace;
    case SDLK_DELETE:
        return InputAction::DeleteKey;
    case SDLK_HOME:
        return InputAction::Home;
    case SDLK_END:
        return InputAction::End;
    case SDLK_TAB:
        return InputAction::Tab;
    case SDLK_F3:
        return InputAction::DebugCollision;
    default:
        return std::nullopt;
    }
}

std::optional<InputAction> KeyboardMouseInputTranslator::action_from_mouse_button(Uint8 button) const
{
    switch (button)
    {
    case SDL_BUTTON_LEFT:
        return InputAction::Attack;
    default:
        return std::nullopt;
    }
}
}
