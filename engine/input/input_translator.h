#pragma once

#include "input_types.h"
#include "../base/singleton.h"

#include <SDL.h>
#include <vector>

class InputTranslator : public Singleton<InputTranslator>
{
    friend Singleton<InputTranslator>;

public:
    std::vector<InputEvent> translate_event(const SDL_Event& event) const;

private:
    InputTranslator() = default;
};
