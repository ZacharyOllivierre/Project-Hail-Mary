#include "input_device_tracker.h"

void InputDeviceTracker::begin_frame()
{
    _device_switched_this_frame = false;
}

InputDeviceUpdateResult InputDeviceTracker::process_event(const SDL_Event& event)
{
    InputDeviceUpdateResult result;

    const InputDevice event_device = detect_event_device(event);
    if (event_device == InputDevice::Unknown)
    {
        return result;
    }

    if (_current_device == InputDevice::Unknown)
    {
        _current_device = event_device;
        return result;
    }

    if (event_device == InputDevice::Gamepad && _current_device != InputDevice::Gamepad)
    {
        _current_device = event_device;
        _device_switched_this_frame = true;
        result.should_translate = false;
        result.should_clear_state = true;
        result.should_reset_gamepad_state = true;
        return result;
    }

    if (is_keyboard_or_mouse(event_device))
    {
        if (_current_device == InputDevice::Gamepad)
        {
            result.should_clear_state = true;
            result.should_reset_gamepad_state = true;
        }

        _current_device = event_device;
    }

    return result;
}

InputDevice InputDeviceTracker::current_device() const
{
    return _current_device;
}

bool InputDeviceTracker::device_switched_this_frame() const
{
    return _device_switched_this_frame;
}

InputDevice InputDeviceTracker::detect_event_device(const SDL_Event& event) const
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
    case SDL_TEXTEDITING:
    case SDL_TEXTINPUT:
        return InputDevice::Keyboard;

    case SDL_MOUSEMOTION:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEWHEEL:
        return InputDevice::Mouse;

    case SDL_CONTROLLERAXISMOTION:
    case SDL_CONTROLLERBUTTONDOWN:
    case SDL_CONTROLLERBUTTONUP:
        return InputDevice::Gamepad;

    default:
        return InputDevice::Unknown;
    }
}

bool InputDeviceTracker::is_keyboard_or_mouse(InputDevice device) const
{
    return device == InputDevice::Keyboard || device == InputDevice::Mouse;
}
