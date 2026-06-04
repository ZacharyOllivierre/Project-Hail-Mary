#pragma once
#include "atlas.h"
#include "../resource_sub_manager.h"
#include "../resource_types.h"

#include <SDL.h>

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class TextureManager;

using AtlasPool = std::unordered_map<std::string, std::unique_ptr<Atlas>>;

class AtlasManager : public ResourceSubManager
{
public:
	explicit AtlasManager(TextureManager& texture_manager);

	bool load_atlas(SDL_Renderer* renderer, const AtlasLoadRequest& request);
	bool load_atlases(
		SDL_Renderer* renderer,
		const std::vector<AtlasLoadRequest>& requests
	);

	Atlas* find_atlas(const std::string_view& key) const;

	void clear() override;
	size_t resource_count() const override;

private:
	bool collect_frame_paths(
		const AtlasLoadRequest& request,
		std::vector<std::filesystem::path>& frame_paths
	) const;
	std::string make_texture_key(
		const std::string& atlas_key,
		size_t frame_index
	) const;

private:
	TextureManager& _texture_manager;
	AtlasPool _atlas_pool;
};
