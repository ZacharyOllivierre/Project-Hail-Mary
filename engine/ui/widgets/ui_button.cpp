#include "ui_button.h"

#include "../../core/render/render_command.h"

#include <algorithm>
#include <utility>

namespace engine::ui
{

UiButton::UiButton(const engine::core::Rect& rect, int order) noexcept
    : UiElement(rect, order)
{
}

UiButton::UiButton(const engine::core::Vector2& position, const engine::core::Vector2& size, int order) noexcept
    : UiElement(position, size, order)
{
}

void UiButton::reset() noexcept
{
    UiElement::reset();
    _state = UiButtonState::Idle;
    _enabled = true;
    _is_pressing = false;
}

bool UiButton::on_input_event(const engine::input::InputEvent& event)
{
    if (!event.has_pointer_position)
        return false;

    const bool inside = contains_pointer(event.pointer_x, event.pointer_y);
    if (!_enabled)
        return false;

    if (event.type == engine::input::InputEventType::PointerMoved)
    {
        set_hovered(inside);
        return inside || _is_pressing;
    }

    if (event.action != engine::input::InputAction::Attack)
        return false;

    if (event.type == engine::input::InputEventType::Pressed && inside)
    {
        _is_pressing = true;
        _state = UiButtonState::Pressed;
        return true;
    }

    if (event.type == engine::input::InputEventType::Released && _is_pressing)
    {
        _is_pressing = false;
        _state = inside ? UiButtonState::Hovered : UiButtonState::Idle;
        if (inside && _on_click)
            _on_click();
        return true;
    }

    return false;
}

void UiButton::submit_ui_render_commands(std::vector<engine::core::UiRenderCommand>& out_commands) const
{
    if (screen_rect().is_empty())
        return;

    if (SDL_Texture* texture = current_state_texture())
        out_commands.push_back(engine::core::make_ui_texture_command(texture, screen_rect()));
    else
    {
        out_commands.push_back(engine::core::make_ui_fill_rect_command(screen_rect(), current_fill_color()));
        out_commands.push_back(engine::core::make_ui_draw_rect_command(screen_rect(), _colors.border));
    }

    if (_content_texture)
        out_commands.push_back(engine::core::make_ui_texture_command(_content_texture, fitted_content_rect()));
}

void UiButton::set_enabled(bool enabled) noexcept
{
    _enabled = enabled;
    _is_pressing = false;
    _state = enabled ? UiButtonState::Idle : UiButtonState::Disabled;
}

bool UiButton::is_enabled() const noexcept { return _enabled; }
UiButtonState UiButton::state() const noexcept { return _state; }
void UiButton::set_on_click(ClickCallback callback) { _on_click = std::move(callback); }
void UiButton::set_colors(const UiButtonColors& colors) noexcept { _colors = colors; }
const UiButtonColors& UiButton::colors() const noexcept { return _colors; }
void UiButton::set_state_textures(const UiButtonTextures& textures) noexcept { _state_textures = textures; }
const UiButtonTextures& UiButton::state_textures() const noexcept { return _state_textures; }

void UiButton::set_content_texture(SDL_Texture* texture, const engine::core::Vector2& size) noexcept
{
    _content_texture = texture;
    _content_size = size;
}

void UiButton::clear_content_texture() noexcept
{
    _content_texture = nullptr;
    _content_size = engine::core::Vector2::zero();
}

void UiButton::set_padding(float padding) noexcept { _padding = std::max(0.0f, padding); }
float UiButton::padding() const noexcept { return _padding; }

bool UiButton::contains_pointer(int x, int y) const noexcept
{
    return screen_rect().contains_half_open(engine::core::Vector2(static_cast<float>(x), static_cast<float>(y)));
}

SDL_Color UiButton::current_fill_color() const noexcept
{
    switch (_state)
    {
    case UiButtonState::Hovered: return _colors.hovered;
    case UiButtonState::Pressed: return _colors.pressed;
    case UiButtonState::Disabled: return _colors.disabled;
    case UiButtonState::Idle: return _colors.idle;
    }
    return _colors.idle;
}

SDL_Texture* UiButton::current_state_texture() const noexcept
{
    switch (_state)
    {
    case UiButtonState::Hovered: return _state_textures.hovered;
    case UiButtonState::Pressed: return _state_textures.pressed;
    case UiButtonState::Disabled: return _state_textures.disabled;
    case UiButtonState::Idle: return _state_textures.idle;
    }
    return nullptr;
}

engine::core::Rect UiButton::content_rect() const noexcept
{
    const engine::core::Rect& button_rect = screen_rect();
    return engine::core::Rect(button_rect.x() + _padding, button_rect.y() + _padding,
        button_rect.width() - _padding * 2.0f, button_rect.height() - _padding * 2.0f);
}

engine::core::Rect UiButton::fitted_content_rect() const noexcept
{
    const engine::core::Rect available = content_rect();
    if (_content_size.x <= 0.0f || _content_size.y <= 0.0f || available.is_empty())
        return available;

    const float scale = std::min(1.0f, std::min(available.width() / _content_size.x, available.height() / _content_size.y));
    const engine::core::Vector2 size = _content_size * scale;
    return engine::core::Rect::from_center(available.center(), size);
}

void UiButton::set_hovered(bool hovered) noexcept
{
    if (!_is_pressing)
        _state = hovered ? UiButtonState::Hovered : UiButtonState::Idle;
}
}
