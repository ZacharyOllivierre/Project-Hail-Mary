#include "resource_bootstrapper.h"

#include "resource_manager.h"

#include "../animation/animation_manager.h"
#include "../io/json_loader.h"
#include "../io/path_manager.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace
{
struct AnimationSettings
{
	double fps = 10.0;
	bool loop = true;
};

bool has_png_extension(const std::filesystem::path& path)
{
	return path.has_extension() && path.extension() == ".png";
}

bool collect_animation_frame_paths(
	const std::filesystem::path& character_root,
	const std::string& character_id,
	const std::string& animation_name,
	std::vector<std::filesystem::path>& out_frame_paths,
	std::string& out_error
)
{
	out_frame_paths.clear();
	out_error.clear();

	const std::filesystem::path animation_directory =
		character_root / character_id / animation_name;

	if (std::filesystem::exists(animation_directory))
	{
		if (!std::filesystem::is_directory(animation_directory))
		{
			out_error = "Animation path is not a directory: "
				+ animation_directory.string();
			return false;
		}

		for (const std::filesystem::directory_entry& entry :
			std::filesystem::directory_iterator(animation_directory))
		{
			if (!entry.is_regular_file())
				continue;

			if (!has_png_extension(entry.path()))
				continue;

			out_frame_paths.push_back(entry.path());
		}

		std::sort(out_frame_paths.begin(), out_frame_paths.end());
		if (out_frame_paths.empty())
		{
			out_error = "Animation directory contains no PNG frames: "
				+ animation_directory.string();
			return false;
		}

		return true;
	}

	const std::filesystem::path single_frame_path =
		character_root / character_id / (character_id + "_" + animation_name + ".png");
	if (std::filesystem::exists(single_frame_path) &&
		std::filesystem::is_regular_file(single_frame_path))
	{
		out_frame_paths.push_back(single_frame_path);
		return true;
	}

	out_error = "Animation assets not found for "
		+ character_id + "." + animation_name
		+ " (expected directory: " + animation_directory.string()
		+ " or file: " + single_frame_path.string() + ")";
	return false;
}
}

bool ResourceBootstrapper::bootstrap(
	ResourceManager& resource_manager,
	SDL_Renderer* renderer
)
{
	if (!renderer)
	{
		std::cout << "Resource bootstrap failed: renderer is empty." << std::endl;
		return false;
	}

	JsonLoader loader;
	const std::filesystem::path config_path =
		PathManager::instance()->configs() / "character_information.json";
	const JsonReadResult open_result = loader.open_file(config_path);
	if (!open_result)
	{
		std::cout << "Load character animation config failed:\n"
			<< open_result.error;
		return false;
	}

	std::vector<std::string> characters;
	std::vector<std::string> animations;

	const JsonReadResult characters_result = loader.get_array("characters", characters);
	if (!characters_result)
	{
		std::cout << "Read characters list failed:\n"
			<< characters_result.error;
		return false;
	}

	const JsonReadResult animations_result = loader.get_array("animation", animations);
	if (!animations_result)
	{
		std::cout << "Read animation list failed:\n"
			<< animations_result.error;
		return false;
	}

	const json* animation_settings_root = nullptr;
	const JsonReadResult settings_root_result =
		loader.get_object("animation_settings", animation_settings_root);
	if (!settings_root_result)
	{
		std::cout << "Read animation settings root failed:\n"
			<< settings_root_result.error;
		return false;
	}

	std::unordered_map<std::string, AnimationSettings> animation_settings;
	for (const std::string& animation_name : animations)
	{
		const json* animation_node = nullptr;
		const JsonReadResult animation_node_result =
			loader.get_object(*animation_settings_root, animation_name, animation_node);
		if (!animation_node_result)
		{
			std::cout << "Read animation settings failed for "
				<< animation_name << ":\n"
				<< animation_node_result.error;
			return false;
		}

		AnimationSettings settings;
		const JsonReadResult fps_result = loader.get(*animation_node, "fps", settings.fps);
		if (!fps_result)
		{
			std::cout << "Read animation fps failed for "
				<< animation_name << ":\n"
				<< fps_result.error;
			return false;
		}

		const JsonReadResult loop_result = loader.get(*animation_node, "loop", settings.loop);
		if (!loop_result)
		{
			std::cout << "Read animation loop flag failed for "
				<< animation_name << ":\n"
				<< loop_result.error;
			return false;
		}

		animation_settings.emplace(animation_name, settings);
	}

	const std::filesystem::path character_root =
		PathManager::instance()->assets() / "character";

	for (const std::string& character_id : characters)
	{
		for (const std::string& animation_name : animations)
		{
			std::vector<std::filesystem::path> frame_paths;
			std::string error;
			if (!collect_animation_frame_paths(
				character_root,
				character_id,
				animation_name,
				frame_paths,
				error))
			{
				std::cout << "Collect animation frames failed: "
					<< error << std::endl;
				return false;
			}

			const std::string animation_key = character_id + "." + animation_name;

			AtlasLoadRequest atlas_request;
			atlas_request._atlas_key = animation_key;
			atlas_request._frame_paths = std::move(frame_paths);

			const Atlas* atlas = resource_manager.build_atlas(renderer, atlas_request);
			if (!atlas)
			{
				std::cout << "Build atlas failed: "
					<< animation_key << std::endl;
				return false;
			}

			const AnimationSettings& settings = animation_settings.at(animation_name);

			AnimationBuildRequest animation_request;
			animation_request._animation_key = animation_key;
			animation_request._fps = settings.fps;
			animation_request._loop = settings.loop;
			if (!AnimationManager::instance()->register_animation(animation_request, atlas))
			{
				std::cout << "Register character animation failed: "
					<< animation_key << std::endl;
				return false;
			}
		}
	}

	return true;
}
