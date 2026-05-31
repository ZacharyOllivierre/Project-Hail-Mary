#pragma once

#include "input_types.h"

#include <array>
#include <cstddef>

class InputState
{
public:
    void begin_frame()
    {
        _previous = _current;
    }

    void set_pressed(InputAction action, bool pressed)
    {
        _current[index(action)] = pressed;
    }

    bool is_pressed(InputAction action) const
    {
        return _current[index(action)];
    }

    bool is_just_pressed(InputAction action) const
    {
        std::size_t i = index(action);
        return _current[i] && !_previous[i];
    }

    bool is_just_released(InputAction action) const
    {
        std::size_t i = index(action);
        return !_current[i] && _previous[i];
    }

private:
    static constexpr std::size_t index(InputAction action)
    {
        return static_cast<std::size_t>(action);
    }

private:
    std::array<bool, static_cast<std::size_t>(InputAction::Count)> _current{};
    std::array<bool, static_cast<std::size_t>(InputAction::Count)> _previous{};
};
