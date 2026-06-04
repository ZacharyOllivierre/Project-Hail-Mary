#include "effect_manager.h"
#include "animation_manager.h"

#include <iostream>

bool EffectManager::register_effect(const std::vector<EffectBuildRequest>& requests)
{
	for (const EffectBuildRequest& request : requests)
	{
		if (!register_effect(request))
			return false;
	}

	return true;
};

bool EffectManager::register_effect(const EffectBuildRequest& request)
{
	if (request._effect_key.empty())
	{
		std::cout << "Register effect failed: effect key is empty." << std::endl;
		return false;
	}

	if (request._animation_key.empty())
	{
		std::cout << "Register effect failed: animation key is empty." << std::endl;
		return false;
	}

	if (! AnimationManager::instance()->find_definition(request._animation_key))
	{
		std::cout << "Register effect failed: can't find animation definition." << std::endl;
		return false;
	}


	EffectDefinition definition;
	definition._effect_key = request._effect_key;
	definition._animation_key = request._animation_key;

	_definitions[request._effect_key] = definition;
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
	const EffectDefinition* definition = find_definition(request._effect_key);

	if (!definition)
	{
		std::cout << "Create effect failed: definition does not exist: "
			<< request._effect_key << std::endl;
		return nullptr;
	}

	std::unique_ptr<Animation> animation =
		AnimationManager::instance()->create_animation(definition->_animation_key);

	if (!animation)
	{
		std::cout << "Create effect failed: animation creation failed: "
			<< definition->_animation_key << std::endl;
		return nullptr;
	}

	std::unique_ptr<Effect> effect = std::make_unique<Effect>(
		definition->_effect_key,
		definition->_animation_key,
		std::move(animation)
	);

	effect->set_world_position(request._position);

	return effect;
}

