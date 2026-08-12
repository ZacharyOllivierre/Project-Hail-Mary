#include "fonts_manifest_loader.h"

#include "../json_loader.h"
#include "../../tools/logger.h"

#include <utility>

namespace engine::io
{

bool FontsManifestLoader::load(
	const std::filesystem::path& manifest_path,
	FontManifest& manifest
) const
{
	manifest = FontManifest{};

	JsonLoader loader;
	const JsonReadResult result = loader.open_file(manifest_path);
	if (!result)
	{
		ENGINE_LOG_ERROR("resource","Load fonts manifest failed: " << result.error);
		return false;
	}

	if (!loader.root().is_object())
	{
		ENGINE_LOG_ERROR("resource","Load fonts manifest failed: root is not an object: "
			<< manifest_path);
		return false;
	}

	if (!loader.root().contains("fonts") || !loader.root().at("fonts").is_array())
	{
		ENGINE_LOG_ERROR("resource","Load fonts manifest failed: fonts is missing or not an array: "
			<< manifest_path);
		return false;
	}

	FontManifest parsed_manifest;
	for (const json& font : loader.root().at("fonts"))
	{
		if (!font.is_object())
		{
			ENGINE_LOG_ERROR("resource","Load fonts manifest failed: font entry is not an object.");
			return false;
		}

		if (!font.contains("key") || !font.at("key").is_string())
		{
			ENGINE_LOG_ERROR("resource","Load fonts manifest failed: key is missing or not a string.");
			return false;
		}

		if (!font.contains("file") || !font.at("file").is_string())
		{
			ENGINE_LOG_ERROR("resource","Load fonts manifest failed: file is missing or not a string.");
			return false;
		}

		if (!font.contains("size") || !font.at("size").is_number_integer())
		{
			ENGINE_LOG_ERROR("resource","Load fonts manifest failed: size is missing or not an integer.");
			return false;
		}

		FontManifestEntry entry;
		entry.key = font.at("key").get<std::string>();
		entry.file_path = font.at("file").get<std::string>();
		entry.point_size = font.at("size").get<int>();
		parsed_manifest.fonts.push_back(std::move(entry));
	}

	manifest = std::move(parsed_manifest);
	return true;
}
}
