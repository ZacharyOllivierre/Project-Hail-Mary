#include "ui_image.h"

#include "../../core/render/render_command.h"

UiImage::UiImage(SDL_Texture* texture, const Rect& rect, int order) noexcept
    : UiElement(rect, order), _texture(texture)
{
}

UiImage::UiImage(SDL_Texture* texture, const Vector2& position, const Vector2& size, int order) noexcept
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

void UiImage::set_source_rect(const Rect& source_rect) noexcept
{
    _source_rect = source_rect;
    _has_source_rect = true;
}

void UiImage::clear_source_rect() noexcept
{
    _source_rect = Rect::zero();
    _has_source_rect = false;
}

bool UiImage::has_source_rect() const noexcept
{
    return _has_source_rect;
}

const Rect& UiImage::source_rect() const noexcept
{
    return _source_rect;
}

void UiImage::submit_ui_render_commands(std::vector<UiRenderCommand>& out_commands) const
{
    if (!_texture || screen_rect().is_empty())
        return;

    UiRenderCommand command = make_ui_texture_command(_texture, screen_rect());
    command.use_src_rect = _has_source_rect;
    command.src_rect = _source_rect;
    out_commands.push_back(command);
}
