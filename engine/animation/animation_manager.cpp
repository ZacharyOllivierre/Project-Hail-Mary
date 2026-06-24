#include "animation_manager.h"

#include "../resources/resource_manager.h"

#include <iostream>

bool AnimationManager::register_animation(
	const AnimationBuildRequest& request,
	const Atlas* atlas
)
{
	if (request.animation_key.empty())
	{
		std::cout << "Register animation failed: animation key is empty." << std::endl;
		return false;
	}

	if (request.atlas_key.empty())
	{
		std::cout << "Register animation failed: atlas key is empty: "
			<< request.animation_key << std::endl;
		return false;
	}

	if (!atlas)
	{
		std::cout << "Register animation failed: atlas is null: "
			<< request.animation_key << std::endl;
		return false;
	}

	if (request.fps <= 0.0)
	{
		std::cout << "Register animation failed: fps is invalid: "
			<< request.animation_key << std::endl;
		return false;
	}

	AnimationDefinition definition;
	definition.animation_key = request.animation_key;
	definition.atlas_key = request.atlas_key;
	definition.fps = request.fps;
	definition.loop = request.loop;
	definition.segment_index = request.segment_index;
	definition.atlas = atlas;

	_definitions[request.animation_key] = definition;
	return true;
}

bool AnimationManager::register_animations(
	const std::vector<AnimationBuildRequest>& requests,
	const ResourceManager& resource_manager
)
{
	for (const AnimationBuildRequest& request : requests)
	{
		const Atlas* atlas = resource_manager.find_atlas(request.atlas_key);
		if (!register_animation(request, atlas))
			return false;
	}

	return true;
}

const AnimationDefinition* AnimationManager::find_definition(const std::string_view& key) const
{
	std::unordered_map<std::string, AnimationDefinition>::const_iterator iterator =
		_definitions.find(std::string(key));
	if (iterator == _definitions.end())
		return nullptr;

	return &iterator->second;
}

std::unique_ptr<Animation> AnimationManager::create_animation(const std::string_view& key) const
{
	const AnimationDefinition* definition = find_definition(key);
	if (!definition)
	{
		std::cout << "Create animation failed: definition does not exist: "
			<< key << std::endl;
		return nullptr;
	}

	std::unique_ptr<Animation> animation = std::make_unique<Animation>();
	animation->set_atlas(definition->atlas);
	animation->set_loop(definition->loop);
	animation->set_interval_seconds(1.0 / definition->fps);
	return animation;
}
