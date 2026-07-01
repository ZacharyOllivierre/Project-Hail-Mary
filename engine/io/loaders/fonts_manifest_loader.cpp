#include "fonts_manifest_loader.h"

#include "../json_loader.h"

#include <iostream>
#include <utility>

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
		std::cout << "Load fonts manifest failed: " << result.error;
		return false;
	}

	if (!loader.root().is_object())
	{
		std::cout << "Load fonts manifest failed: root is not an object: "
			<< manifest_path << std::endl;
		return false;
	}

	if (!loader.root().contains("fonts") || !loader.root().at("fonts").is_array())
	{
		std::cout << "Load fonts manifest failed: fonts is missing or not an array: "
			<< manifest_path << std::endl;
		return false;
	}

	FontManifest parsed_manifest;
	for (const json& font : loader.root().at("fonts"))
	{
		if (!font.is_object())
		{
			std::cout << "Load fonts manifest failed: font entry is not an object."
				<< std::endl;
			return false;
		}

		if (!font.contains("key") || !font.at("key").is_string())
		{
			std::cout << "Load fonts manifest failed: key is missing or not a string."
				<< std::endl;
			return false;
		}

		if (!font.contains("file") || !font.at("file").is_string())
		{
			std::cout << "Load fonts manifest failed: file is missing or not a string."
				<< std::endl;
			return false;
		}

		if (!font.contains("size") || !font.at("size").is_number_integer())
		{
			std::cout << "Load fonts manifest failed: size is missing or not an integer."
				<< std::endl;
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
