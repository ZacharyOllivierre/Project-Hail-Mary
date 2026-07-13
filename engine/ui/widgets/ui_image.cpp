#include "ui_image.h"

#include "../../core/render/render_command.h"

namespace engine::ui
{

UiImage::UiImage(SDL_Texture* texture, const engine::core::Rect& rect, int order) noexcept
    : UiElement(rect, order), _texture(texture)
{
}

UiImage::UiImage(SDL_Texture* texture, const engine::core::Vector2& position, const engine::core::Vector2& size, int order) noexcept
    : UiElement(position, size, order), _texture(texture)
{
}

void UiImage::set_texture(SDL_Texture* texture) noexcept
{
    _texture = texture;
}

SDL_Texture* UiImage::texture() const noexcept
{
    return _texture;
}

void UiImage::set_source_rect(const engine::core::Rect& source_rect) noexcept
{
    _source_rect = source_rect;
    _has_source_rect = true;
}

void UiImage::clear_source_rect() noexcept
{
    _source_rect = engine::core::Rect::zero();
    _has_source_rect = false;
}

bool UiImage::has_source_rect() const noexcept
{
    return _has_source_rect;
}

const engine::core::Rect& UiImage::source_rect() const noexcept
{
    return _source_rect;
}

void UiImage::submit_ui_render_commands(std::vector<engine::core::UiRenderCommand>& out_commands) const
{
    if (!_texture || screen_rect().is_empty())
        return;

    engine::core::UiRenderCommand command = engine::core::make_ui_texture_command(_texture, screen_rect());
    command.use_src_rect = _has_source_rect;
    command.src_rect = _source_rect;
    out_commands.push_back(command);
}
}
