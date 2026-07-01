#pragma once

#include "asset_config_types.h"

#include <filesystem>

class AudioManifestLoader
{
public:
	bool load(
		const std::filesystem::path& manifest_path,
		AudioManifest& manifest
	) const;
};
