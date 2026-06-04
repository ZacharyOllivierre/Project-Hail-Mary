#pragma once
#include "../../io/loaders/asset_config_types.h"
#include "../resource_types.h"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

class FileManager
{
public:
	bool load_assets_strcutre(const std::filesystem::path& assets_structure_path);
	bool load_configs();
	bool load_textures();
	bool load_audio();
	bool load_i18n();

	const std::vector<AtlasLoadRequest>& atlas_load_requests() const;
	const std::vector<AnimationBuildRequest>& animation_build_requests() const;

private:
	bool load_character_configs(const std::filesystem::path& manifest_path);

private:
	std::vector<AssetDirectoryEntry> _directories;
	std::unordered_map<std::string, std::filesystem::path> _manifest_paths;
	std::vector<AtlasLoadRequest> _atlas_load_requests;
	std::vector<AnimationBuildRequest> _animation_build_requests;
	bool _find_all_folder = false;
};
