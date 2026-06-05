#include "gamepad_input_translator.h"

#include <SDL.h>

std::vector<InputEvent> GamepadInputTranslator::translate_event(const SDL_Event& event) const
{
    std::vector<InputEvent> events;

    switch (event.type)
    {
    case SDL_CONTROLLERBUTTONDOWN:
    case SDL_CONTROLLERBUTTONUP:
        append_controller_button_events(
            events,
            event.cbutton.button,
            input_event_type(event.type == SDL_CONTROLLERBUTTONDOWN)
        );
        break;

    default:
        break;
    }

    return events;
}

void GamepadInputTranslator::append_controller_button_events(
    std::vector<InputEvent>& events,
    Uint8 button,
    InputEventType type
) const
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
    case SDL_CONTROLLER_BUTTON_START:
        append_event(events, InputAction::Pause, type, InputDevice::Gamepad);
        break;
    default:
        break;
    }
}
