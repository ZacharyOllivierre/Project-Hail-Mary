#pragma once

#include "../input_types.h"

namespace engine::input
{

class InputEventReceiver
{
public:
    virtual ~InputEventReceiver() = default;
    virtual bool on_input_event(const InputEvent& event) = 0;
};
}
