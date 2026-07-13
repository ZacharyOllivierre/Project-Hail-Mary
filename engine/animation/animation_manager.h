#pragma once

#include "animation.h"
#include "../tools/singleton.h"
#include "../resources/resource_types.h"

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace engine::resources
{
class Atlas;
class ResourceManager;
}

namespace engine::animation
{



struct AnimationDefinition
{
	std::string animation_key;
	std::string atlas_key;
	double fps = 10.0;
	bool loop = true;
	size_t segment_index = 0;
	const ::engine::resources::Atlas* atlas = nullptr;
};

class AnimationManager : public ::engine::tools::Singleton<AnimationManager>
{
	friend ::engine::tools::Singleton<AnimationManager>;

public:
	bool register_animation(
		const ::engine::resources::AnimationBuildRequest& request,
		const ::engine::resources::Atlas* atlas
	);
	bool register_animations(
		const std::vector<::engine::resources::AnimationBuildRequest>& requests,
		const ::engine::resources::ResourceManager& resource_manager
	);

	const AnimationDefinition* find_definition(const std::string_view& key) const;
	std::unique_ptr<Animation> create_animation(const std::string_view& key) const;

private:
	std::unordered_map<std::string, AnimationDefinition> _definitions;
};
}
