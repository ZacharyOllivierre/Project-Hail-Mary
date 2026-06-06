#pragma once

#include "../../input/contracts/input_event_receiver.h"

class UiFocusable : public InputEventReceiver
{
public:
    virtual ~UiFocusable() = default;

    virtual void set_focused(bool focused) = 0;
    [[nodiscard]] virtual bool is_focused() const = 0;
};
