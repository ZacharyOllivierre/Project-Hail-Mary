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
	std::string _animation_key;
	std::string _atlas_key;
	double _fps = 10.0;
	bool _loop = true;
	size_t _segment_index = 0;
	const Atlas* _atlas = nullptr;
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
