#include "audio_manifest_loader.h"

#include "../json_loader.h"

#include <iostream>
#include <string>
#include <utility>

namespace
{
bool append_audio_entries_from_object(
	const json& node,
	const char* group_name,
	std::vector<AudioManifestEntry>& out_entries
)
{
	if (!node.is_object())
	{
		std::cout << "Load audio manifest failed: " << group_name
			<< " is not an object." << std::endl;
		return false;
	}

	for (json::const_iterator item = node.begin(); item != node.end(); ++item)
	{
		if (!item.value().is_object())
		{
			std::cout << "Load audio manifest failed: entry is not an object: "
				<< item.key() << std::endl;
			return false;
		}

		const json& entry_node = item.value();
		if (!entry_node.contains("path") || !entry_node.at("path").is_string())
		{
			std::cout << "Load audio manifest failed: path is missing or not a string: "
				<< item.key() << std::endl;
			return false;
		}

		AudioManifestEntry entry;
		entry.key = item.key();
		entry.file_path = entry_node.at("path").get<std::string>();
		out_entries.push_back(std::move(entry));
	}

	return true;
}
}

bool AudioManifestLoader::load(
	const std::filesystem::path& manifest_path,
	AudioManifest& manifest
) const
{
	manifest = AudioManifest{};

	JsonLoader loader;
	const JsonReadResult result = loader.open_file(manifest_path);
	if (!result)
	{
		std::cout << "Load audio manifest failed: " << result.error;
		return false;
	}

	if (!loader.root().is_object())
	{
		std::cout << "Load audio manifest failed: root is not an object: "
			<< manifest_path << std::endl;
		return false;
	}

	if (!loader.root().contains("sounds"))
	{
		std::cout << "Load audio manifest failed: sounds is missing: "
			<< manifest_path << std::endl;
		return false;
	}

	if (!loader.root().contains("music"))
	{
		std::cout << "Load audio manifest failed: music is missing: "
			<< manifest_path << std::endl;
		return false;
	}

	AudioManifest parsed_manifest;
	if (!append_audio_entries_from_object(
		loader.root().at("sounds"),
		"sounds",
		parsed_manifest.sounds))
	{
		return false;
	}

	if (!append_audio_entries_from_object(
		loader.root().at("music"),
		"music",
		parsed_manifest.music))
	{
		return false;
	}

	manifest = std::move(parsed_manifest);
	return true;
}
