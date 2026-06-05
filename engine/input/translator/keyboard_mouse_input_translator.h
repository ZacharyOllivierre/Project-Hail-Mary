#pragma once

#include "input_translator.h"

#include <optional>

class KeyboardMouseInputTranslator : public InputTranslator
{
public:
    std::vector<InputEvent> translate_event(const SDL_Event& event) const override;

private:
    std::optional<InputAction> action_from_key(SDL_Keycode key) const;
    std::optional<InputAction> action_from_mouse_button(Uint8 button) const;
};
