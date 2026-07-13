#pragma once

#include "../core/ui_element.h"

struct SDL_Texture;

// A non-owning texture widget rendered through the scene UI command pass.
class UiImage : public UiElement
{
public:
    UiImage(SDL_Texture* texture, const Rect& rect, int order = 0) noexcept;
    UiImage(SDL_Texture* texture, const Vector2& position, const Vector2& size, int order = 0) noexcept;

    void set_texture(SDL_Texture* texture) noexcept;
    [[nodiscard]] SDL_Texture* texture() const noexcept;

    void set_source_rect(const Rect& source_rect) noexcept;
    void clear_source_rect() noexcept;
    [[nodiscard]] bool has_source_rect() const noexcept;
    [[nodiscard]] const Rect& source_rect() const noexcept;

    void submit_ui_render_commands(std::vector<UiRenderCommand>& out_commands) const override;

private:
    SDL_Texture* _texture = nullptr;
    Rect _source_rect{};
    bool _has_source_rect = false;
};
