#include "ui_animation.h"

#include "../../animation/animation.h"
#include "../../animation/animation_manager.h"
#include "../../core/render/render_command.h"
#include "../../tools/logger.h"

#include <utility>

namespace engine::ui
{

UiAnimation::UiAnimation(
    std::string_view animation_key,
    const engine::core::Rect& rect,
    int order
)
    : UiElement(rect, order)
{
    if (!set_animation(animation_key))
        ENGINE_LOG_WARN("ui", "Create UiAnimation failed: " << animation_key);
}

UiAnimation::UiAnimation(
    std::string_view animation_key,
    const engine::core::Vector2& position,
    const engine::core::Vector2& size,
    int order
)
    : UiElement(position, size, order)
{
    if (!set_animation(animation_key))
        ENGINE_LOG_WARN("ui", "Create UiAnimation failed: " << animation_key);
}

UiAnimation::~UiAnimation() = default;

bool UiAnimation::set_animation(std::string_view key)
{
    std::unique_ptr<engine::animation::Animation> animation =
        engine::animation::AnimationManager::instance()->create_animation(key);
    if (!animation)
        return false;

    _animation_key = key;
    _animation = std::move(animation);
    return true;
}

void UiAnimation::play()
{
    if (!_animation)
        return;

    if (_animation->is_finished())
        _animation->reset();
    else
        _animation->resume();
}

void UiAnimation::pause()
{
    if (_animation)
        _animation->pause();
}

void UiAnimation::reset() noexcept
{
    UiElement::reset();
    if (_animation)
        _animation->reset();
}

void UiAnimation::update(double delta)
{
    if (_animation)
        _animation->update(delta);
}

void UiAnimation::submit_ui_render_commands(
    std::vector<engine::core::UiRenderCommand>& out_commands
) const
{
    if (!_animation || screen_rect().is_empty())
        return;

    const engine::resources::FrameInfo* frame = _animation->current_frame();
    if (!frame || !frame->_texture)
        return;

    out_commands.push_back(
        engine::core::make_ui_texture_command(frame->_texture, screen_rect()));
}

bool UiAnimation::has_animation() const noexcept
{
    return _animation != nullptr;
}

const std::string& UiAnimation::animation_key() const noexcept
{
    return _animation_key;
}

}
