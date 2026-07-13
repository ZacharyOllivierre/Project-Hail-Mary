#include "effect_manager.h"
#include "animation_manager.h"

#include <iostream>

namespace engine::animation
{

namespace
{
std::optional<engine::core::Vector2> resolve_effect_size(
	const EffectSpawnRequest& request,
	const EffectDefinition& definition
)
{
	if (request.size.has_value())
		return request.size;

	if (!definition.default_size.is_zero())
		return definition.default_size;

	return std::nullopt;
}

engine::core::Vector2 get_effect_top_left(
	const engine::core::Vector2& anchor_position,
	const engine::core::Vector2& size,
	EffectAnchor anchor
)
{
	switch (anchor)
	{
	case EffectAnchor::TopLeft:
		return anchor_position;

	case EffectAnchor::TopCenter:
		return engine::core::Vector2(anchor_position.x - size.x * 0.5f, anchor_position.y);

	case EffectAnchor::TopRight:
		return engine::core::Vector2(anchor_position.x - size.x, anchor_position.y);

	case EffectAnchor::CenterLeft:
		return engine::core::Vector2(anchor_position.x, anchor_position.y - size.y * 0.5f);

	case EffectAnchor::Center:
		return engine::core::Vector2(anchor_position.x - size.x * 0.5f, anchor_position.y - size.y * 0.5f);

	case EffectAnchor::CenterRight:
		return engine::core::Vector2(anchor_position.x - size.x, anchor_position.y - size.y * 0.5f);

	case EffectAnchor::BottomLeft:
		return engine::core::Vector2(anchor_position.x, anchor_position.y - size.y);

	case EffectAnchor::BottomCenter:
		return engine::core::Vector2(anchor_position.x - size.x * 0.5f, anchor_position.y - size.y);

	case EffectAnchor::BottomRight:
		return engine::core::Vector2(anchor_position.x - size.x, anchor_position.y - size.y);

	default:
		return anchor_position;
	}
}

void apply_effect_anchor(Effect& effect, const EffectSpawnRequest& request)
{
	effect.set_position(get_effect_top_left(request.position, effect.size(), request.anchor));
}
}

bool EffectManager::register_effect(const std::vector<engine::resources::EffectBuildRequest>& requests)
{
	for (const engine::resources::EffectBuildRequest& request : requests)
	{
		if (!register_effect(request))
			return false;
	}

	return true;
};

bool EffectManager::register_effect(const engine::resources::EffectBuildRequest& request)
{
	if (request.effect_key.empty())
	{
		std::cout << "Register effect failed: effect key is empty." << std::endl;
		return false;
	}

	if (request.animation_key.empty())
	{
		std::cout << "Register effect failed: animation key is empty." << std::endl;
		return false;
	}

	if (!AnimationManager::instance()->find_definition(request.animation_key))
	{
		std::cout << "Register effect failed: can't find animation definition." << std::endl;
		return false;
	}


	EffectDefinition definition;
	definition.effect_key = request.effect_key;
	definition.animation_key = request.animation_key;
	if (request.default_size.has_value())
		definition.default_size = *request.default_size;
	if (request.angle_degrees.has_value())
		definition.angle_degrees = *request.angle_degrees;

	_definitions[request.effect_key] = definition;
	return true;
};

const EffectDefinition* EffectManager::find_definition(const std::string_view& key) const
{
	std::unordered_map<std::string, EffectDefinition>::const_iterator iterator =
		_definitions.find(std::string(key));
	if (iterator == _definitions.end())
		return nullptr;

	return &iterator->second;
}

std::unique_ptr<Effect> EffectManager::create_effect(const EffectSpawnRequest& request) const
{
	const EffectDefinition* definition = find_definition(request.effect_key);

	if (!definition)
	{
		std::cout << "Create effect failed: definition does not exist: "
			<< request.effect_key << std::endl;
		return nullptr;
	}

	std::unique_ptr<Animation> animation =
		AnimationManager::instance()->create_animation(definition->animation_key);

	if (!animation)
	{
		std::cout << "Create effect failed: animation creation failed: "
			<< definition->animation_key << std::endl;
		return nullptr;
	}

	std::unique_ptr<Effect> effect = std::make_unique<Effect>(
		definition->effect_key,
		definition->animation_key,
		std::move(animation)
	);

	const std::optional<engine::core::Vector2> final_size = resolve_effect_size(request, *definition);
	if (final_size.has_value())
		effect->set_size(*final_size);

	apply_effect_anchor(*effect, request);

	if (request.angle_degrees.has_value())
		effect->set_angle(*request.angle_degrees);
	else
		effect->set_angle(definition->angle_degrees);

	if (request.flip.has_value())
		effect->set_flip(*request.flip);
	else
		effect->set_flip(engine::core::SpriteFlip::None);

	return effect;
}
}
