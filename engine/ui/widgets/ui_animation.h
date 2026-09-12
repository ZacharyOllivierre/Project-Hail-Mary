#pragma once

#include "../core/ui_element.h"
#include "../../core/interface/updatable.h"

#include <memory>
#include <string>
#include <string_view>

namespace engine::animation
{
class Animation;
}

namespace engine::ui
{

// A screen-space animation widget backed by an AnimationManager instance.
class UiAnimation final : public UiElement, public engine::core::Updatable
{
public:
    explicit UiAnimation(
        std::string_view animation_key,
        const engine::core::Rect& rect = engine::core::Rect::zero(),
        int order = 0
    );
    UiAnimation(
        std::string_view animation_key,
        const engine::core::Vector2& position,
        const engine::core::Vector2& size,
        int order = 0
    );
    ~UiAnimation() override;

    // Replaces the current animation only when the key can be resolved.
    [[nodiscard]] bool set_animation(std::string_view key);
    void play();
    void pause();
    void reset() noexcept override;

    void update(double delta) override;
    void submit_ui_render_commands(
        std::vector<engine::core::UiRenderCommand>& out_commands
    ) const override;

    [[nodiscard]] bool has_animation() const noexcept;
    [[nodiscard]] const std::string& animation_key() const noexcept;

private:
    std::string _animation_key;
    std::unique_ptr<engine::animation::Animation> _animation;
};

}
