#include "file_manager.h"

#include "../../io/loaders/animation_config_loader.h"
#include "../../io/loaders/assets_structure_loader.h"
#include "../../io/loaders/character_animation_layout_loader.h"
#include "../../io/loaders/character_config_loader.h"
#include "../../io/loaders/character_manifest_loader.h"
#include "resource_request_builder.h"

#include <iostream>
#include <utility>

bool FileManager::load_assets_strcutre(const std::filesystem::path& assets_structure_path)
{
	_directories.clear();
	_manifest_paths.clear();
	_atlas_load_requests.clear();
	_animation_build_requests.clear();
	_find_all_folder = false;

	AssetsStructureManifest manifest;
	AssetsStructureLoader loader;
	if (!loader.load(assets_structure_path, manifest))
		return false;

	_directories = std::move(manifest._directories);
	_manifest_paths = std::move(manifest._manifest_paths);
	_find_all_folder = true;
	return true;
}

bool FileManager::load_configs()
{
	_atlas_load_requests.clear();
	_animation_build_requests.clear();

	if (!_find_all_folder)
	{
		std::cout << "Load configs failed: assets structure is not loaded." << std::endl;
		return false;
	}

	std::unordered_map<std::string, std::filesystem::path>::const_iterator iterator =
		_manifest_paths.find("characters");
	if (iterator == _manifest_paths.end())
	{
		std::cout << "Load configs failed: characters manifest is missing." << std::endl;
		return false;
	}

	if (!load_character_configs(iterator->second))
		return false;

	return true;
}

bool FileManager::load_textures()
{
	return true;
}

bool FileManager::load_audio()
{
	return true;
}

bool FileManager::load_i18n()
{
	return true;
}

const std::vector<AtlasLoadRequest>& FileManager::atlas_load_requests() const
{
	return _atlas_load_requests;
}

const std::vector<AnimationBuildRequest>& FileManager::animation_build_requests() const
{
	return _animation_build_requests;
}

bool FileManager::load_character_configs(const std::filesystem::path& manifest_path)
{
	std::unordered_map<std::string, std::filesystem::path>::const_iterator layout_iterator =
		_manifest_paths.find("character_animations");
	if (layout_iterator == _manifest_paths.end())
	{
		std::cout << "Load character configs failed: character animation layout is missing."
			<< std::endl;
		return false;
	}

	CharacterAnimationLayout character_animation_layout;
	CharacterAnimationLayoutLoader character_animation_layout_loader;
	if (!character_animation_layout_loader.load(
		layout_iterator->second,
		character_animation_layout))
	{
		return false;
	}

	CharacterManifest character_manifest;
	CharacterManifestLoader character_manifest_loader;
	if (!character_manifest_loader.load(manifest_path, character_manifest))
		return false;

	CharacterConfigLoader character_config_loader;
	AnimationConfigLoader animation_config_loader;
	ResourceRequestBuilder request_builder;

	for (const CharacterManifestEntry& character_entry : character_manifest._characters)
	{
		CharacterConfig character_config;
		if (!character_config_loader.load(character_entry, character_config))
			return false;

		AnimationConfig animation_config;
		if (!animation_config_loader.load(
			character_config._animation_config_path,
			character_animation_layout,
			animation_config))
		{
			return false;
		}

		if (!request_builder.append_character_animation_requests(
			character_config,
			animation_config,
			_atlas_load_requests,
			_animation_build_requests))
		{
			return false;
		}
	}

	return true;
}
