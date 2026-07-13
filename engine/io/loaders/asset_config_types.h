#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace engine::io
{

struct FontManifestEntry
{
	std::string key;
	std::filesystem::path file_path;
	int point_size = 0;
};

struct FontManifest
{
	std::vector<FontManifestEntry> fonts;
};

struct AudioManifestEntry
{
	std::string key;
	std::filesystem::path file_path;
};

struct AudioManifest
{
	std::vector<AudioManifestEntry> sounds;
	std::vector<AudioManifestEntry> music;
};

struct TextureManifestEntry
{
	std::string key;
	std::filesystem::path file_path;
};

struct TextureManifest
{
	std::vector<TextureManifestEntry> textures;
};

struct ConfigRegistry
{
	const std::filesystem::path* find(const std::string_view& key) const
	{
		std::unordered_map<std::string, std::filesystem::path>::const_iterator iterator =
			config_paths.find(std::string(key));
		if (iterator == config_paths.end())
			return nullptr;

		return &iterator->second;
	}

	std::unordered_map<std::string, std::filesystem::path> config_paths;
};
}
