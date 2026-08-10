#include "animation_effect_factory.h"

#include "../../animation/animation_manager.h"

#include <SDL.h>

#include <algorithm>
#include <optional>
#include <utility>

namespace engine::effects
{
namespace
{

std::optional<engine::core::Vector2> resolve_effect_size(
	const AnimationEffectSpawnRequest& request,
	const AnimationEffectDefinition& definition,
	const engine::animation::Animation& animation)
{
	if (request.size.has_value())
		return request.size;

	if (!definition.default_size.is_zero())
		return definition.default_size;

	const engine::resources::FrameInfo* frame = animation.current_frame();
	if (!frame || frame->_width <= 0 || frame->_height <= 0)
		return std::nullopt;

	return engine::core::Vector2(
		static_cast<float>(frame->_width),
		static_cast<float>(frame->_height));
}

engine::core::Vector2 get_effect_top_left(
	const engine::core::Vector2& anchor_position,
	const engine::core::Vector2& size,
	EffectAnchor anchor)
{
	switch (anchor)
	{
	case EffectAnchor::TopLeft:
		return anchor_position;
	case EffectAnchor::TopCenter:
		return {anchor_position.x - size.x * 0.5f, anchor_position.y};
	case EffectAnchor::TopRight:
		return {anchor_position.x - size.x, anchor_position.y};
	case EffectAnchor::CenterLeft:
		return {anchor_position.x, anchor_position.y - size.y * 0.5f};
	case EffectAnchor::Center:
		return {anchor_position.x - size.x * 0.5f, anchor_position.y - size.y * 0.5f};
	case EffectAnchor::CenterRight:
		return {anchor_position.x - size.x, anchor_position.y - size.y * 0.5f};
	case EffectAnchor::BottomLeft:
		return {anchor_position.x, anchor_position.y - size.y};
	case EffectAnchor::BottomCenter:
		return {anchor_position.x - size.x * 0.5f, anchor_position.y - size.y};
	case EffectAnchor::BottomRight:
		return {anchor_position.x - size.x, anchor_position.y - size.y};
	default:
		return anchor_position;
	}
}

}

std::unique_ptr<AnimationEffect> AnimationEffectFactory::create(
	const AnimationEffectSpawnRequest& request,
	const AnimationEffectDefinition& definition) const
{
	std::unique_ptr<engine::animation::Animation> animation =
		engine::animation::AnimationManager::instance()->create_animation(
			definition.animation_key);
	if (!animation)
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
			"Create animation effect failed: animation creation failed: %s",
			definition.animation_key.c_str());
		return nullptr;
	}

	const std::optional<engine::core::Vector2> final_size =
		resolve_effect_size(request, definition, *animation);
	auto effect = std::make_unique<AnimationEffect>(
		definition.effect_key, definition.animation_key, std::move(animation));

	if (final_size.has_value())
		effect->set_size(*final_size);

	effect->set_position(get_effect_top_left(request.position, effect->size(), request.anchor));
	effect->set_angle(request.angle_degrees.value_or(definition.angle_degrees));
	effect->set_flip(request.flip.value_or(engine::core::SpriteFlip::None));
	effect->set_start_delay(std::max(0.0, request.start_delay_seconds));
	effect->set_on_started(request.on_started);
	effect->set_on_finished(request.on_finished);
	for (const AnimationEffectSpawnRequest::ScheduledCallbackRequest& callback
		 : request.scheduled_callbacks)
	{
		effect->schedule_callback(callback.delay_seconds, callback.callback);
	}

	return effect;
}

}
