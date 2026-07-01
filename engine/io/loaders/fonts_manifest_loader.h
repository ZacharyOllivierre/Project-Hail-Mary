#pragma once

#include "asset_config_types.h"

#include <filesystem>

class FontsManifestLoader
{
public:
	bool load(
		const std::filesystem::path& manifest_path,
		FontManifest& manifest
	) const;
};
