#pragma once

#include "input_types.h"

#include <SDL.h>

struct InputDeviceUpdateResult
{
    bool should_translate = true;
    bool should_clear_state = false;
    bool should_reset_gamepad_state = false;
};

class InputDeviceTracker
{
public:
    void begin_frame();
    InputDeviceUpdateResult process_event(const SDL_Event& event);
    InputDevice current_device() const;
    bool device_switched_this_frame() const;

private:
    InputDevice detect_event_device(const SDL_Event& event) const;
    bool is_keyboard_or_mouse(InputDevice device) const;

private:
    InputDevice _current_device = InputDevice::Unknown;
    bool _device_switched_this_frame = false;
};
