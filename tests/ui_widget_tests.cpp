#include "../engine/input/translator/keyboard_mouse_input_translator.h"
#include "../engine/ui/widgets/ui_button.h"
#include "../engine/ui/widgets/ui_image.h"

#include <cassert>
#include <vector>

namespace
{
using engine::core::Rect;
using engine::core::UiRenderCommand;
using engine::core::Vector2;
using engine::input::InputAction;
using engine::input::InputDevice;
using engine::input::InputEvent;
using engine::input::InputEventType;
using engine::input::KeyboardMouseInputTranslator;
using engine::ui::UiButton;
using engine::ui::UiButtonState;
using engine::ui::UiButtonTextures;
using engine::ui::UiImage;

InputEvent pointer_event(InputEventType type, int x, int y)
{
    InputEvent event;
    event.action = type == InputEventType::PointerMoved ? InputAction::Count : InputAction::Attack;
    event.type = type;
    event.device = InputDevice::Mouse;
    event.has_pointer_position = true;
    event.pointer_x = x;
    event.pointer_y = y;
    return event;
}
}

int main()
{
    SDL_Texture* texture = reinterpret_cast<SDL_Texture*>(0x1);
    UiImage image(texture, Rect(10.0f, 20.0f, 30.0f, 40.0f));
    std::vector<UiRenderCommand> commands;
    image.submit_ui_render_commands(commands);
    assert(commands.size() == 1 && commands.front().texture == texture);
    image.set_source_rect(Rect(1.0f, 2.0f, 3.0f, 4.0f));
    commands.clear();
    image.submit_ui_render_commands(commands);
    assert(commands.front().use_src_rect && commands.front().src_rect == Rect(1.0f, 2.0f, 3.0f, 4.0f));

    UiButton button(Rect(10.0f, 10.0f, 100.0f, 40.0f));
    button.submit_ui_render_commands(commands);
    assert(commands.size() == 3); // image command plus fill and border
    commands.clear();
    int clicks = 0;
    button.set_on_click([&clicks] { ++clicks; });
    assert(button.on_input_event(pointer_event(InputEventType::PointerMoved, 20, 20)));
    assert(button.state() == UiButtonState::Hovered);
    assert(button.on_input_event(pointer_event(InputEventType::Pressed, 20, 20)));
    assert(button.state() == UiButtonState::Pressed);
    assert(button.on_input_event(pointer_event(InputEventType::Released, 20, 20)));
    assert(clicks == 1);
    assert(button.on_input_event(pointer_event(InputEventType::Pressed, 20, 20)));
    assert(button.on_input_event(pointer_event(InputEventType::Released, 200, 200)));
    assert(clicks == 1);
    button.set_enabled(false);
    assert(!button.on_input_event(pointer_event(InputEventType::Pressed, 20, 20)));
    assert(button.state() == UiButtonState::Disabled);

    UiButton textured_button(Rect(0.0f, 0.0f, 100.0f, 40.0f));
    textured_button.set_state_textures(UiButtonTextures{ texture, texture, texture, texture });
    textured_button.set_content_texture(texture, Vector2(20.0f, 10.0f));
    commands.clear();
    textured_button.submit_ui_render_commands(commands);
    assert(commands.size() == 2 && commands.front().texture == texture);
    assert(commands.back().screen_rect == Rect(40.0f, 15.0f, 20.0f, 10.0f));

    KeyboardMouseInputTranslator translator;
    SDL_Event motion{};
    motion.type = SDL_MOUSEMOTION;
    motion.motion.x = 12;
    motion.motion.y = 34;
    const auto moved = translator.translate_event(motion);
    assert(moved.size() == 1 && moved.front().type == InputEventType::PointerMoved);
    assert(moved.front().has_pointer_position && moved.front().pointer_x == 12 && moved.front().pointer_y == 34);

    return 0;
}
