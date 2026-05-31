#include "input_system.h"
#include "input_translator.h"

void InputSystem::begin_frame()
{
    _state.begin_frame();
    _events.clear();
}

void InputSystem::process_event(const SDL_Event& event)
{
    update_current_device(event);
    translate_event(event);
}

InputSnapshot InputSystem::snapshot() const
{
    return { _state, _context, _current_device };
}

const std::vector<InputEvent>& InputSystem::events() const
{
    return _events;
}

InputDevice InputSystem::current_device() const
{
    return _current_device;
}

void InputSystem::set_context(InputContext context)
{
    _context = context;
}

InputContext InputSystem::context() const
{
    return _context;
}

void InputSystem::translate_event(const SDL_Event& event)
{
    std::vector<InputEvent> input_events = InputTranslator::instance()->translate_event(event);

    for (const InputEvent& input_event : input_events)
    {
        apply_event(input_event);
        _current_device = input_event.device;
        _events.push_back(input_event);
    }
}

void InputSystem::apply_event(const InputEvent& event)
{
    _state.set_pressed(event.action, event.type == InputEventType::Pressed);
}

void InputSystem::update_current_device(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
    case SDL_TEXTEDITING:
    case SDL_TEXTINPUT:
        _current_device = InputDevice::Keyboard;
        break;
    case SDL_MOUSEMOTION:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEWHEEL:
        _current_device = InputDevice::Mouse;
        break;
    case SDL_CONTROLLERAXISMOTION:
    case SDL_CONTROLLERBUTTONDOWN:
    case SDL_CONTROLLERBUTTONUP:
    case SDL_CONTROLLERDEVICEADDED:
    case SDL_CONTROLLERDEVICEREMOVED:
    case SDL_CONTROLLERDEVICEREMAPPED:
        _current_device = InputDevice::Gamepad;
        break;
    default:
        break;
    }
}
