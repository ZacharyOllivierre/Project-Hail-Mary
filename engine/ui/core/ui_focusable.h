#pragma once

#include "../../input/contracts/input_event_receiver.h"

namespace engine::ui
{

class UiFocusable : public ::engine::input::InputEventReceiver
{
public:
    virtual ~UiFocusable() = default;

    virtual void set_focused(bool focused) = 0;
    [[nodiscard]] virtual bool is_focused() const = 0;
};
}
