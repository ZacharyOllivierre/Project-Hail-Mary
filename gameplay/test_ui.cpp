#include "test_ui.h"

#include "../engine/core/render/render_command.h"

TestUi::TestUi() : UiElement(Rect{ 40, 40, 220, 120 })
{
}

void TestUi::submit_ui_render_commands(std::vector<UiRenderCommand>& out_commands) const
{
	out_commands.push_back(make_ui_fill_rect_command(screen_rect(), _color));
}
