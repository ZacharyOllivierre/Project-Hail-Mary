#pragma once

#include "atlas.h"
#include "../resource_types.h"
#include "../texture/texture_loader.h"

#include <vector>

class AtlasBuilder
{
public:
	bool build_atlas(
		const AtlasLoadRequest& request,
		const std::vector<TextureLoadResult>& texture_results,
		Atlas& atlas
	) const;
};
