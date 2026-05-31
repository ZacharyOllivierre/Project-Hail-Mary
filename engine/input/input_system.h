#pragma once

#include "input_state.h"
#include "input_types.h"

#include <SDL.h>
#include <vector>

struct InputSnapshot
{
    const InputState& state;
    InputContext context = InputContext::Gameplay;
    InputDevice device = InputDevice::Unknown;
};

class InputSystem
{
public:
    void begin_frame();
    void process_event(const SDL_Event& event);
    InputSnapshot snapshot() const;
    const std::vector<InputEvent>& events() const;
    InputDevice current_device() const;
    void set_context(InputContext context);
    InputContext context() const;

private:
    void translate_event(const SDL_Event& event);
    void apply_event(const InputEvent& event);
    void update_current_device(const SDL_Event& event);

private:
    InputState _state;
    std::vector<InputEvent> _events;
    InputContext _context = InputContext::Gameplay;
    InputDevice _current_device = InputDevice::Unknown;
};
