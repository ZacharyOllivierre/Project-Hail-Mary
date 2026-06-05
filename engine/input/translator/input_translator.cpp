#include "input_translator.h"

InputEventType InputTranslator::input_event_type(bool pressed) const
{
    return pressed ? InputEventType::Pressed : InputEventType::Released;
}

void InputTranslator::append_event(
    std::vector<InputEvent>& events,
    InputAction action,
    InputEventType type,
    InputDevice device
) const
{
    events.push_back({ action, type, device });
}
