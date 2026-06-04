#include "animation_manager.h"

#include "../resources/resource_manager.h"

#include <iostream>

bool AnimationManager::register_animation(
	const AnimationBuildRequest& request,
	const Atlas* atlas
)
{
	if (request._animation_key.empty())
	{
		std::cout << "Register animation failed: animation key is empty." << std::endl;
		return false;
	}

	if (request._atlas_key.empty())
	{
		std::cout << "Register animation failed: atlas key is empty: "
			<< request._animation_key << std::endl;
		return false;
	}

	if (!atlas)
	{
		std::cout << "Register animation failed: atlas is null: "
			<< request._animation_key << std::endl;
		return false;
	}

	if (request._fps <= 0.0)
	{
		std::cout << "Register animation failed: fps is invalid: "
			<< request._animation_key << std::endl;
		return false;
	}

	AnimationDefinition definition;
	definition._animation_key = request._animation_key;
	definition._atlas_key = request._atlas_key;
	definition._fps = request._fps;
	definition._loop = request._loop;
	definition._segment_index = request._segment_index;
	definition._atlas = atlas;

	_definitions[request._animation_key] = definition;
	return true;
}

bool AnimationManager::register_animations(
	const std::vector<AnimationBuildRequest>& requests,
	const ResourceManager& resource_manager
)
{
	for (const AnimationBuildRequest& request : requests)
	{
		const Atlas* atlas = resource_manager.find_atlas(request._atlas_key);
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
	animation->set_atlas(definition->_atlas);
	animation->set_loop(definition->_loop);
	animation->set_interval_seconds(1.0 / definition->_fps);
	return animation;
}

