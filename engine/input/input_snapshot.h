#pragma once

#include "input_types.h"

namespace engine::input
{

class InputState;

struct InputSnapshot
{
    const InputState& state;
    InputContext context = InputContext::Gameplay;
    InputDevice device = InputDevice::Unknown;
    bool device_switched_this_frame = false;
};
}
