#pragma once

#include "atlas.h"
#include "../resource_types.h"

#include <SDL.h>

#include <string>
#include <string_view>
#include <unordered_map>

namespace engine::resources
{

class TextureManager;

using AtlasPool = std::unordered_map<std::string, Atlas>;

class AtlasManager
{
public:
	AtlasManager() = default;

	bool load_atlas(
		SDL_Renderer* renderer,
		const AtlasLoadRequest& request,
		TextureManager& texture_manager
	);
	const Atlas* find_atlas(const std::string_view& key) const;

private:
	AtlasPool _atlas_pool;
};
}
