#pragma once

#include "../input_types.h"

#include <SDL.h>
#include <vector>

class InputTranslator
{
public:
    virtual ~InputTranslator() = default;
    virtual std::vector<InputEvent> translate_event(const SDL_Event& event) const = 0;

protected:
    InputEventType input_event_type(bool pressed) const;
    void append_event(
        std::vector<InputEvent>& events,
        InputAction action,
        InputEventType type,
        InputDevice device
    ) const;
};
