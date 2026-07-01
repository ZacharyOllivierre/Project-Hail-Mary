#pragma once

#include "asset_config_types.h"

#include <filesystem>

class TextureManifestLoader
{
public:
	bool load(
		const std::filesystem::path& manifest_path,
		TextureManifest& manifest
	) const;
};
