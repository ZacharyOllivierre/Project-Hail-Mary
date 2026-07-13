#pragma once

#include "../core/ui_element.h"
#include "../../input/contracts/input_event_receiver.h"

#include <SDL.h>
#include <functional>

struct SDL_Texture;

namespace engine::ui
{

enum class UiButtonState
{
    Idle,
    Hovered,
    Pressed,
    Disabled
};

struct UiButtonColors
{
    SDL_Color idle{180, 180, 180, 255};
    SDL_Color hovered{200, 200, 200, 255};
    SDL_Color pressed{130, 130, 130, 255};
    SDL_Color disabled{100, 100, 100, 255};
    SDL_Color border{0, 0, 0, 255};
};

struct UiButtonTextures
{
    SDL_Texture* idle = nullptr;
    SDL_Texture* hovered = nullptr;
    SDL_Texture* pressed = nullptr;
    SDL_Texture* disabled = nullptr;
};

// A pointer-operated UI button. Textures are caller-owned and never released here.
class UiButton : public UiElement, public engine::input::InputEventReceiver
{
public:
    using ClickCallback = std::function<void()>;

    UiButton(const engine::core::Rect& rect = engine::core::Rect::zero(), int order = 0) noexcept;
    UiButton(const engine::core::Vector2& position, const engine::core::Vector2& size, int order = 0) noexcept;

    void reset() noexcept override;
    bool on_input_event(const engine::input::InputEvent& event) override;
    void submit_ui_render_commands(std::vector<engine::core::UiRenderCommand>& out_commands) const override;

    void set_enabled(bool enabled) noexcept;
    [[nodiscard]] bool is_enabled() const noexcept;
    [[nodiscard]] UiButtonState state() const noexcept;

    void set_on_click(ClickCallback callback);
    void set_colors(const UiButtonColors& colors) noexcept;
    [[nodiscard]] const UiButtonColors& colors() const noexcept;
    void set_state_textures(const UiButtonTextures& textures) noexcept;
    [[nodiscard]] const UiButtonTextures& state_textures() const noexcept;

    // Content uses the supplied pixel size and is scaled down to fit inside padding.
    void set_content_texture(SDL_Texture* texture, const engine::core::Vector2& size) noexcept;
    void clear_content_texture() noexcept;
    void set_padding(float padding) noexcept;
    [[nodiscard]] float padding() const noexcept;

private:
    [[nodiscard]] bool contains_pointer(int x, int y) const noexcept;
    [[nodiscard]] SDL_Color current_fill_color() const noexcept;
    [[nodiscard]] SDL_Texture* current_state_texture() const noexcept;
    [[nodiscard]] engine::core::Rect content_rect() const noexcept;
    [[nodiscard]] engine::core::Rect fitted_content_rect() const noexcept;
    void set_hovered(bool hovered) noexcept;

private:
    UiButtonState _state = UiButtonState::Idle;
    UiButtonColors _colors{};
    UiButtonTextures _state_textures{};
    SDL_Texture* _content_texture = nullptr;
    engine::core::Vector2 _content_size{};
    ClickCallback _on_click;
    float _padding = 5.0f;
    bool _enabled = true;
    bool _is_pressing = false;
};
}
