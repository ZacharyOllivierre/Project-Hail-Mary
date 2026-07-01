#include "texture_manifest_loader.h"

#include "../json_loader.h"

#include <iostream>
#include <utility>

bool TextureManifestLoader::load(
	const std::filesystem::path& manifest_path,
	TextureManifest& manifest
) const
{
	manifest = TextureManifest{};

	JsonLoader loader;
	const JsonReadResult result = loader.open_file(manifest_path);
	if (!result)
	{
		std::cout << "Load texture manifest failed: " << result.error;
		return false;
	}

	if (!loader.root().is_object())
	{
		std::cout << "Load texture manifest failed: root is not an object: "
			<< manifest_path << std::endl;
		return false;
	}

	if (!loader.root().contains("textures") || !loader.root().at("textures").is_object())
	{
		std::cout << "Load texture manifest failed: textures is missing or not an object: "
			<< manifest_path << std::endl;
		return false;
	}

	TextureManifest parsed_manifest;
	for (json::const_iterator texture = loader.root().at("textures").begin();
		texture != loader.root().at("textures").end();
		++texture)
	{
		if (!texture.value().is_object())
		{
			std::cout << "Load texture manifest failed: texture entry is not an object: "
				<< texture.key() << std::endl;
			return false;
		}

		const json& texture_node = texture.value();
		if (!texture_node.contains("path") || !texture_node.at("path").is_string())
		{
			std::cout << "Load texture manifest failed: path is missing or not a string: "
				<< texture.key() << std::endl;
			return false;
		}

		TextureManifestEntry entry;
		entry.key = texture.key();
		entry.file_path = texture_node.at("path").get<std::string>();
		parsed_manifest.textures.push_back(std::move(entry));
	}

	manifest = std::move(parsed_manifest);
	return true;
}
