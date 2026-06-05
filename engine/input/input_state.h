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

    void clear()
    {
        _current.fill(false);
        _previous.fill(false);
    }

    void set_pressed(InputAction action, bool pressed)
    {
        if (!is_trackable_action(action))
        {
            return;
        }

        _current[index(action)] = pressed;
    }

    bool is_pressed(InputAction action) const
    {
        if (!is_trackable_action(action))
        {
            return false;
        }

        return _current[index(action)];
    }

    bool is_just_pressed(InputAction action) const
    {
        if (!is_trackable_action(action))
        {
            return false;
        }

        std::size_t i = index(action);
        return _current[i] && !_previous[i];
    }

    bool is_just_released(InputAction action) const
    {
        if (!is_trackable_action(action))
        {
            return false;
        }

        std::size_t i = index(action);
        return !_current[i] && _previous[i];
    }

private:
    static constexpr bool is_trackable_action(InputAction action)
    {
        return action != InputAction::Count;
    }

    static constexpr std::size_t index(InputAction action)
    {
        return static_cast<std::size_t>(action);
    }

private:
    std::array<bool, static_cast<std::size_t>(InputAction::Count)> _current{};
    std::array<bool, static_cast<std::size_t>(InputAction::Count)> _previous{};
};
