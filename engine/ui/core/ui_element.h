#pragma once

#include <vector>

#include "../../core/scene_object.h"
#include "../../core/geometry/vector2.h"
#include "../../core/geometry/rect.h"

namespace engine::core
{
struct UiRenderCommand;
}

namespace engine::ui
{

struct UiTheme;


class UiElement : public engine::core::SceneObject
{
public:

    // Higher UI order values are rendered on top and receive input first.
    explicit UiElement(
        const engine::core::Rect& rect = engine::core::Rect::zero(),
        int order = 0
    ) noexcept : _screen_rect(rect), _order(order) {}

    UiElement(
        const engine::core::Vector2& position,
        const engine::core::Vector2& size,
        int order = 0
    ) noexcept : _screen_rect(position, size), _order(order) {}

    virtual ~UiElement() = default;

    UiElement(const UiElement&) = delete;
    UiElement& operator=(const UiElement&) = delete;
    UiElement(UiElement&&) = delete;
    UiElement& operator=(UiElement&&) = delete;

    virtual void submit_ui_render_commands(std::vector<engine::core::UiRenderCommand>& out_commands) const
    {
        (void)out_commands;
    }

    void reset() noexcept override
    {
        engine::core::SceneObject::reset();
        _use_theme = true;
    }

    void set_screen_rect(const engine::core::Rect& rect) noexcept { _screen_rect = rect; }
    void set_position(const engine::core::Vector2& position) noexcept { _screen_rect.set_position(position); }
    void set_center(const engine::core::Vector2& center) noexcept { _screen_rect.set_center(center); }
    void set_size(const engine::core::Vector2& size) noexcept { _screen_rect.set_size(size); }

    [[nodiscard]] const engine::core::Rect& screen_rect() const noexcept { return _screen_rect; }
    [[nodiscard]] engine::core::Vector2 position() const noexcept { return _screen_rect.position(); }
    [[nodiscard]] engine::core::Vector2 center() const noexcept { return _screen_rect.center(); }
    [[nodiscard]] engine::core::Vector2 size() const noexcept { return _screen_rect.size(); }

    [[nodiscard]] int order() const noexcept { return _order; }

    void set_use_theme(bool use_theme) noexcept { _use_theme = use_theme; }
    [[nodiscard]] bool uses_theme() const noexcept { return _use_theme; }

    [[nodiscard]] bool update_when_paused() const override{ return true;}

    [[nodiscard]] bool receive_input_when_paused() const override{ return true;}

protected:
    virtual void apply_theme(const UiTheme& theme) { (void)theme; }

private:
    engine::core::Rect _screen_rect{};
    int _order = 0;
    bool _use_theme = true;
};
}
