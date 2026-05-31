#include "input_translator.h"

#include <optional>

InputEventType input_event_type(bool pressed)
{
    return pressed ? InputEventType::Pressed : InputEventType::Released;
}

void append_event(std::vector<InputEvent>& events, InputAction action, InputEventType type, InputDevice device)
{
    events.push_back({ action, type, device });
}

std::optional<InputAction> action_from_key(SDL_Keycode key)
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
    case SDLK_SPACE:
        return InputAction::Jump;
    case SDLK_j:
        return InputAction::Attack;
    case SDLK_k:
        return InputAction::Special;
    case SDLK_l:
        return InputAction::Guard;
    case SDLK_LSHIFT:
    case SDLK_RSHIFT:
        return InputAction::Dash;
    default:
        return std::nullopt;
    }
}

std::optional<InputAction> action_from_mouse_button(Uint8 button)
{
    switch (button)
    {
    case SDL_BUTTON_LEFT:
        return InputAction::Attack;
    case SDL_BUTTON_RIGHT:
        return InputAction::Guard;
    default:
        return std::nullopt;
    }
}

void append_controller_button_events(std::vector<InputEvent>& events, Uint8 button, InputEventType type)
{
    switch (static_cast<SDL_GameControllerButton>(button))
    {
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        append_event(events, InputAction::Left, type, InputDevice::Gamepad);
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        append_event(events, InputAction::Right, type, InputDevice::Gamepad);
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_UP:
        append_event(events, InputAction::Up, type, InputDevice::Gamepad);
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        append_event(events, InputAction::Down, type, InputDevice::Gamepad);
        break;
    case SDL_CONTROLLER_BUTTON_A:
        append_event(events, InputAction::Confirm, type, InputDevice::Gamepad);
        append_event(events, InputAction::Jump, type, InputDevice::Gamepad);
        break;
    case SDL_CONTROLLER_BUTTON_B:
        append_event(events, InputAction::Cancel, type, InputDevice::Gamepad);
        break;
    case SDL_CONTROLLER_BUTTON_X:
        append_event(events, InputAction::Attack, type, InputDevice::Gamepad);
        break;
    case SDL_CONTROLLER_BUTTON_Y:
        append_event(events, InputAction::Special, type, InputDevice::Gamepad);
        break;
    case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
        append_event(events, InputAction::Guard, type, InputDevice::Gamepad);
        break;
    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
        append_event(events, InputAction::Dash, type, InputDevice::Gamepad);
        break;
    case SDL_CONTROLLER_BUTTON_START:
        append_event(events, InputAction::Pause, type, InputDevice::Gamepad);
        break;
    default:
        break;
    }
}

std::vector<InputEvent> InputTranslator::translate_event(const SDL_Event& event) const
{
    std::vector<InputEvent> events;

    switch (event.type)
    {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
    {
        if (event.type == SDL_KEYDOWN && event.key.repeat != 0)
            return events;

        std::optional<InputAction> action = action_from_key(event.key.keysym.sym);
        if (action)
            append_event(events, *action, input_event_type(event.type == SDL_KEYDOWN), InputDevice::Keyboard);

        break;
    }
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    {
        std::optional<InputAction> action = action_from_mouse_button(event.button.button);
        if (action)
            append_event(events, *action, input_event_type(event.type == SDL_MOUSEBUTTONDOWN), InputDevice::Mouse);

        break;
    }
    case SDL_CONTROLLERBUTTONDOWN:
    case SDL_CONTROLLERBUTTONUP:
        append_controller_button_events(events, event.cbutton.button, input_event_type(event.type == SDL_CONTROLLERBUTTONDOWN));
        break;
    case SDL_QUIT:
        append_event(events, InputAction::Exit, InputEventType::Pressed, InputDevice::Unknown);
        break;
    default:
        break;
    }

    return events;
}
