#include "input_translator.h"

InputEventType InputTranslator::input_event_type(bool pressed) const
{
    return pressed ? InputEventType::Pressed : InputEventType::Released;
}

void InputTranslator::append_event(
    std::vector<InputEvent>& events,
    InputAction action,
    InputEventType type,
    InputDevice device,
    int pointer_x,
    int pointer_y,
    bool has_pointer_position
) const
{
    InputEvent event;
    event.action = action;
    event.type = type;
    event.device = device;
    event.has_pointer_position = has_pointer_position;
    event.pointer_x = pointer_x;
    event.pointer_y = pointer_y;
    events.push_back(event);
}
