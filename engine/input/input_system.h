#pragma once

#include "input_snapshot.h"
#include "input_state.h"
#include "input_types.h"
#include "input_device_tracker.h"

#include "translator/gamepad_input_translator.h"
#include "translator/keyboard_mouse_input_translator.h"
#include "translator/input_translator.h"

#include <SDL.h>
#include <vector>

class InputSystem
{
public:
    void begin_frame();
    void end_frame();
    void process_event(const SDL_Event& event);
    InputSnapshot snapshot() const;
    const std::vector<InputEvent>& events() const;
    InputDevice current_device() const;
    void set_context(InputContext context);
    InputContext context() const;

private:
    void translate_event(const SDL_Event& event);
    const InputTranslator* select_translator(InputDevice device) const;
    void apply_event(const InputEvent& event);
    void append_event(const InputEvent& event);

private:
    InputState _state;
    std::vector<InputEvent> _events;
    InputContext _context = InputContext::Gameplay;
    InputDeviceTracker _device_tracker;
    KeyboardMouseInputTranslator _keyboard_mouse_translator;
    GamepadInputTranslator _gamepad_translator;
};
