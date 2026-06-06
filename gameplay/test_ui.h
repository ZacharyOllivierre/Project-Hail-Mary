#pragma once

#include "../engine/ui/core/ui_element.h"

#include <SDL.h>

class TestUi : public UiElement
{
public:
	TestUi();
	~TestUi() override = default;

	void submit_ui_render_commands(std::vector<UiRenderCommand>& out_commands) const override;

private:
	SDL_Color _color{ 0, 100, 200, 255 };
};
