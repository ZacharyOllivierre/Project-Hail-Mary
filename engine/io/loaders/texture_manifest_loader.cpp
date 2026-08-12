#include "texture_manifest_loader.h"

#include "../json_loader.h"
#include "../../tools/logger.h"

#include <utility>

namespace engine::io
{

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
		ENGINE_LOG_ERROR("resource","Load texture manifest failed: " << result.error);
		return false;
	}

	if (!loader.root().is_object())
	{
		ENGINE_LOG_ERROR("resource","Load texture manifest failed: root is not an object: "
			<< manifest_path);
		return false;
	}

	if (!loader.root().contains("textures") || !loader.root().at("textures").is_object())
	{
		ENGINE_LOG_ERROR("resource","Load texture manifest failed: textures is missing or not an object: "
			<< manifest_path);
		return false;
	}

	TextureManifest parsed_manifest;
	for (json::const_iterator texture = loader.root().at("textures").begin();
		texture != loader.root().at("textures").end();
		++texture)
	{
		if (!texture.value().is_object())
		{
			ENGINE_LOG_ERROR("resource","Load texture manifest failed: texture entry is not an object: "
				<< texture.key());
			return false;
		}

		const json& texture_node = texture.value();
		if (!texture_node.contains("path") || !texture_node.at("path").is_string())
		{
			ENGINE_LOG_ERROR("resource","Load texture manifest failed: path is missing or not a string: "
				<< texture.key());
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
}
