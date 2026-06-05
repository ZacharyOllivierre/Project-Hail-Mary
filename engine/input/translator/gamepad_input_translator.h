#pragma once

#include "input_translator.h"

class GamepadInputTranslator : public InputTranslator
{
public:
    std::vector<InputEvent> translate_event(const SDL_Event& event) const override;

private:
    void append_controller_button_events(
        std::vector<InputEvent>& events,
        Uint8 button,
        InputEventType type
    ) const;
};
