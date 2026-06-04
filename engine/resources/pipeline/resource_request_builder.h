#pragma once
#include "../../io/loaders/asset_config_types.h"
#include "../resource_types.h"

#include <vector>

class ResourceRequestBuilder
{
public:
	bool append_character_animation_requests(
		const CharacterConfig& character_config,
		const AnimationConfig& animation_config,
		std::vector<AtlasLoadRequest>& atlas_load_requests,
		std::vector<AnimationBuildRequest>& animation_build_requests
	) const;
};
