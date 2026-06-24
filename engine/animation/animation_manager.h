#pragma once

#include "animation.h"
#include "../tools/singleton.h"
#include "../resources/resource_types.h"

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class ResourceManager;

struct AnimationDefinition
{
	std::string animation_key;
	std::string atlas_key;
	double fps = 10.0;
	bool loop = true;
	size_t segment_index = 0;
	const Atlas* atlas = nullptr;
};

class AnimationManager : public Singleton<AnimationManager>
{
	friend Singleton<AnimationManager>;

public:
	bool register_animation(
		const AnimationBuildRequest& request,
		const Atlas* atlas
	);
	bool register_animations(
		const std::vector<AnimationBuildRequest>& requests,
		const ResourceManager& resource_manager
	);

	const AnimationDefinition* find_definition(const std::string_view& key) const;
	std::unique_ptr<Animation> create_animation(const std::string_view& key) const;

private:
	std::unordered_map<std::string, AnimationDefinition> _definitions;
};
