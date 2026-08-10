#pragma once

#include "animation/animation_effect.h"

#include <optional>
#include <string>
#include <vector>

namespace engine::effects
{

enum class EffectAnchor
{
	TopLeft,
	TopCenter,
	TopRight,
	CenterLeft,
	Center,
	CenterRight,
	BottomLeft,
	BottomCenter,
	BottomRight
};

struct AnimationEffectDefinition
{
	std::string effect_key;
	std::string animation_key;
	double angle_degrees = 0.0;
	engine::core::Vector2 default_size;
};

struct AnimationEffectSpawnRequest
{
	struct ScheduledCallbackRequest
	{
		double delay_seconds = 0.0;
		AnimationEffect::Callback callback;
	};

	std::string effect_key;
	engine::core::Vector2 position;
	EffectAnchor anchor = EffectAnchor::TopLeft;
	std::optional<engine::core::Vector2> size;
	std::optional<double> angle_degrees;
	std::optional<engine::core::SpriteFlip> flip;
	double start_delay_seconds = 0.0;
	AnimationEffect::Callback on_started;
	AnimationEffect::Callback on_finished;
	std::vector<ScheduledCallbackRequest> scheduled_callbacks;
};

}
