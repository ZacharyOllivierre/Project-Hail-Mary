#include "resource_bootstrapper.h"

#include "resource_manager.h"

#include "../animation/animation_manager.h"
#include "../effects/runtime/effect_manager.h"
#include "../io/loaders/audio_manifest_loader.h"
#include "../io/loaders/configs_list_loader.h"
#include "../io/loaders/fonts_manifest_loader.h"
#include "../io/loaders/texture_manifest_loader.h"
#include "../io/json_loader.h"
#include "../io/path_manager.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace engine::resources
{
using engine::io::AudioManifest;
using engine::io::AudioManifestEntry;
using engine::io::AudioManifestLoader;
using engine::io::ConfigRegistry;
using engine::io::ConfigsListLoader;
using engine::io::FontManifest;
using engine::io::FontManifestEntry;
using engine::io::FontsManifestLoader;
using engine::io::JsonReadResult;
using engine::io::TextureManifest;
using engine::io::TextureManifestEntry;
using engine::io::TextureManifestLoader;
using engine::io::json;

namespace
{
struct AnimationSettings
{
	double fps = 10.0;
	bool loop = true;
};

struct EffectConfig
{
	std::string effect_key;
	std::string animation_key;
	std::filesystem::path directory_path;
	double fps = 10.0;
	bool loop = true;
	std::optional<engine::core::Vector2> default_size;
	std::optional<double> angle_degrees;
};

bool resolve_required_config_path(
	const ConfigRegistry& registry,
	const std::string_view& config_key,
	std::filesystem::path& out_path
)
{
	const std::filesystem::path* config_path = registry.find(config_key);
	if (!config_path)
	{
		std::cout << "Resource bootstrap failed: config is missing from configs_list.json: "
			<< config_key << std::endl;
		return false;
	}

	out_path = *config_path;
	return true;
}

std::filesystem::path resolve_resource_path(
	const std::filesystem::path& root_path,
	const std::filesystem::path& relative_path
)
{
	if (relative_path.is_absolute())
		return relative_path.lexically_normal();

	return (root_path / relative_path).lexically_normal();
}

bool has_png_extension(const std::filesystem::path& path)
{
	return path.has_extension() && path.extension() == ".png";
}

bool collect_png_frame_paths(
	const std::filesystem::path& directory_path,
	std::vector<std::filesystem::path>& out_frame_paths,
	std::string& out_error
)
{
	out_frame_paths.clear();
	out_error.clear();

	if (!std::filesystem::exists(directory_path))
	{
		out_error = "Directory does not exist: " + directory_path.string();
		return false;
	}

	if (!std::filesystem::is_directory(directory_path))
	{
		out_error = "Path is not a directory: " + directory_path.string();
		return false;
	}

	for (const std::filesystem::directory_entry& entry :
		std::filesystem::directory_iterator(directory_path))
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
		out_error = "Directory contains no PNG frames: "
			+ directory_path.string();
		return false;
	}

	return true;
}

bool collect_animation_frame_paths(
	const std::filesystem::path& character_root,
	const std::string& character_id,
	const std::string& animation_name,
	std::vector<std::filesystem::path>& out_frame_paths,
	std::string& out_error
)
{
	const std::filesystem::path animation_directory =
		character_root / character_id / animation_name;

	if (std::filesystem::exists(animation_directory))
		return collect_png_frame_paths(animation_directory, out_frame_paths, out_error);

	const std::filesystem::path single_frame_path =
		character_root / character_id / (character_id + "_" + animation_name + ".png");
	if (std::filesystem::exists(single_frame_path) &&
		std::filesystem::is_regular_file(single_frame_path))
	{
		out_frame_paths.push_back(single_frame_path);
		return true;
	}

	out_error = "engine::animation::Animation assets not found for "
		+ character_id + "." + animation_name
		+ " (expected directory: " + animation_directory.string()
		+ " or file: " + single_frame_path.string() + ")";
	return false;
}

bool read_vector2(
	const engine::io::JsonLoader& loader,
	const json& node,
	std::string_view key,
	engine::core::Vector2& out,
	std::string& out_error
)
{
	const json* vector_node = nullptr;
	const JsonReadResult vector_node_result = loader.get_object(node, key, vector_node);
	if (!vector_node_result)
	{
		out_error = vector_node_result.error;
		return false;
	}

	if (!loader.get(*vector_node, "x", out.x))
	{
		out_error = "JSON value type mismatch: "
			+ std::string(key) + ".x";
		return false;
	}

	if (!loader.get(*vector_node, "y", out.y))
	{
		out_error = "JSON value type mismatch: "
			+ std::string(key) + ".y";
		return false;
	}

	return true;
}

bool load_fonts(
	ResourceManager& resource_manager,
	const std::filesystem::path& config_path
)
{
	FontsManifestLoader loader;
	FontManifest manifest;
	if (!loader.load(config_path, manifest))
		return false;

	const std::filesystem::path font_root = engine::io::PathManager::instance()->fonts();
	for (const FontManifestEntry& entry : manifest.fonts)
	{
		const std::filesystem::path file_path =
			resolve_resource_path(font_root, entry.file_path);
		if (!resource_manager.load_font(
			entry.key,
			file_path,
			entry.point_size))
		{
			std::cout << "Load font failed during bootstrap: "
				<< entry.key << std::endl;
			return false;
		}
	}

	return true;
}

bool load_audio(
	ResourceManager& resource_manager,
	const std::filesystem::path& config_path
)
{
	AudioManifestLoader loader;
	AudioManifest manifest;
	if (!loader.load(config_path, manifest))
		return false;

	const std::filesystem::path audio_root = engine::io::PathManager::instance()->audio();
	for (const AudioManifestEntry& entry : manifest.sounds)
	{
		const std::filesystem::path file_path =
			resolve_resource_path(audio_root, entry.file_path);
		if (!resource_manager.load_sound(entry.key, file_path))
		{
			std::cout << "Load sound failed during bootstrap: "
				<< entry.key << std::endl;
			return false;
		}
	}

	for (const AudioManifestEntry& entry : manifest.music)
	{
		const std::filesystem::path file_path =
			resolve_resource_path(audio_root, entry.file_path);
		if (!resource_manager.load_music(entry.key, file_path))
		{
			std::cout << "Load music failed during bootstrap: "
				<< entry.key << std::endl;
			return false;
		}
	}

	return true;
}

bool load_textures(
	ResourceManager& resource_manager,
	SDL_Renderer* renderer,
	const std::filesystem::path& config_path
)
{
	TextureManifestLoader loader;
	TextureManifest manifest;
	if (!loader.load(config_path, manifest))
		return false;

	const std::filesystem::path texture_root = engine::io::PathManager::instance()->textures();
	for (const TextureManifestEntry& entry : manifest.textures)
	{
		const std::filesystem::path file_path =
			resolve_resource_path(texture_root, entry.file_path);
		if (!resource_manager.load_texture(renderer, entry.key, file_path))
		{
			std::cout << "Load texture failed during bootstrap: "
				<< entry.key << std::endl;
			return false;
		}
	}

	return true;
}

bool load_character_animations(
	const std::filesystem::path& config_path,
	const std::function<const Atlas*(const AtlasLoadRequest&)>& build_atlas
)
{
	engine::io::JsonLoader loader;
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
		engine::io::PathManager::instance()->assets() / "character";

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
			atlas_request.atlas_key = animation_key;
			atlas_request.frame_paths = std::move(frame_paths);

			const Atlas* atlas = build_atlas(atlas_request);
			if (!atlas)
			{
				std::cout << "Build atlas failed: "
					<< animation_key << std::endl;
				return false;
			}

			const AnimationSettings& settings = animation_settings.at(animation_name);

			AnimationBuildRequest animation_request;
			animation_request.animation_key = animation_key;
			animation_request.atlas_key = animation_key;
			animation_request.fps = settings.fps;
			animation_request.loop = settings.loop;
			if (!engine::animation::AnimationManager::instance()->register_animation(animation_request, atlas))
			{
				std::cout << "Register character animation failed: "
					<< animation_key << std::endl;
				return false;
			}
		}
	}

	return true;
}

bool load_effects(
	const std::filesystem::path& config_path,
	const std::function<const Atlas*(const AtlasLoadRequest&)>& build_atlas
)
{
	engine::io::JsonLoader loader;
	const JsonReadResult open_result = loader.open_file(config_path);
	if (!open_result)
	{
		std::cout << "Load effect config failed:\n"
			<< open_result.error;
		return false;
	}

	const json& root = loader.root();
	if (!root.is_object())
	{
		std::cout << "Read effect config failed:\n"
			<< "JSON root is not an object." << std::endl;
		return false;
	}

	if (!root.contains("effects"))
	{
		std::cout << "Read effect config failed:\n"
			<< "JSON key missing: effects" << std::endl;
		return false;
	}

	const json& effects_node = root.at("effects");
	if (!effects_node.is_array())
	{
		std::cout << "Read effect config failed:\n"
			<< "JSON value is not an array: effects" << std::endl;
		return false;
	}

	std::vector<AnimationEffectBuildRequest> effect_requests;
	effect_requests.reserve(effects_node.size());

	for (size_t index = 0; index < effects_node.size(); ++index)
	{
		const json& effect_node = effects_node.at(index);
		if (!effect_node.is_object())
		{
			std::cout << "Read effect config failed:\n"
				<< "engine::effects::AnimationEffect entry is not an object at index " << index << std::endl;
			return false;
		}

		EffectConfig effect_config;

		const JsonReadResult effect_key_result =
			loader.get(effect_node, "effect_key", effect_config.effect_key);
		if (!effect_key_result)
		{
			std::cout << "Read effect key failed at index " << index << ":\n"
				<< effect_key_result.error;
			return false;
		}

		const JsonReadResult animation_key_result =
			loader.get(effect_node, "animation_key", effect_config.animation_key);
		if (!animation_key_result)
		{
			std::cout << "Read effect animation key failed for "
				<< effect_config.effect_key << ":\n"
				<< animation_key_result.error;
			return false;
		}

		const JsonReadResult directory_result =
			loader.get(effect_node, "directory_path", effect_config.directory_path);
		if (!directory_result)
		{
			std::cout << "Read effect directory failed for "
				<< effect_config.effect_key << ":\n"
				<< directory_result.error;
			return false;
		}

		const JsonReadResult fps_result =
			loader.get(effect_node, "fps", effect_config.fps);
		if (!fps_result)
		{
			std::cout << "Read effect fps failed for "
				<< effect_config.effect_key << ":\n"
				<< fps_result.error;
			return false;
		}

		const JsonReadResult loop_result =
			loader.get(effect_node, "loop", effect_config.loop);
		if (!loop_result)
		{
			std::cout << "Read effect loop flag failed for "
				<< effect_config.effect_key << ":\n"
				<< loop_result.error;
			return false;
		}

		if (effect_node.contains("default_size"))
		{
			engine::core::Vector2 default_size;
			std::string error;
			if (!read_vector2(loader, effect_node, "default_size", default_size, error))
			{
				std::cout << "Read effect default size failed for "
					<< effect_config.effect_key << ":\n"
					<< error << std::endl;
				return false;
			}

			effect_config.default_size = default_size;
		}

		if (effect_node.contains("angle_degrees"))
		{
			double angle_degrees = 0.0;
			const JsonReadResult angle_result =
				loader.get(effect_node, "angle_degrees", angle_degrees);
			if (!angle_result)
			{
				std::cout << "Read effect angle failed for "
					<< effect_config.effect_key << ":\n"
					<< angle_result.error;
				return false;
			}

			effect_config.angle_degrees = angle_degrees;
		}

		std::vector<std::filesystem::path> frame_paths;
		std::string error;
		const std::filesystem::path directory_path =
			engine::io::PathManager::instance()->resolve_asset_path(effect_config.directory_path);
		if (!collect_png_frame_paths(directory_path, frame_paths, error))
		{
			std::cout << "Collect effect frames failed for "
				<< effect_config.effect_key << ": "
				<< error << std::endl;
			return false;
		}

		AtlasLoadRequest atlas_request;
		atlas_request.atlas_key = effect_config.animation_key;
		atlas_request.frame_paths = std::move(frame_paths);

		const Atlas* atlas = build_atlas(atlas_request);
		if (!atlas)
		{
			std::cout << "Build effect atlas failed: "
				<< effect_config.animation_key << std::endl;
			return false;
		}

		AnimationBuildRequest animation_request;
		animation_request.animation_key = effect_config.animation_key;
		animation_request.atlas_key = effect_config.animation_key;
		animation_request.fps = effect_config.fps;
		animation_request.loop = effect_config.loop;
		if (!engine::animation::AnimationManager::instance()->register_animation(animation_request, atlas))
		{
			std::cout << "Register effect animation failed: "
				<< effect_config.animation_key << std::endl;
			return false;
		}

		AnimationEffectBuildRequest effect_request;
		effect_request.effect_key = std::move(effect_config.effect_key);
		effect_request.animation_key = std::move(effect_config.animation_key);
		effect_request.default_size = effect_config.default_size.value_or(
			engine::core::Vector2::zero());
		effect_request.default_angle_degrees = effect_config.angle_degrees.value_or(0.0);
		effect_requests.push_back(std::move(effect_request));
	}

	if (!engine::effects::EffectManager::instance()->register_animation_effect(effect_requests))
	{
		std::cout << "Register effect definitions failed." << std::endl;
		return false;
	}

	return true;
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

	const std::function<const Atlas*(const AtlasLoadRequest&)> build_atlas =
		[&resource_manager, renderer](const AtlasLoadRequest& request)
		{
			return resource_manager.build_atlas(renderer, request);
		};

	ConfigsListLoader configs_list_loader;
	ConfigRegistry config_registry;
	if (!configs_list_loader.load(engine::io::PathManager::instance()->configs_list(), config_registry))
		return false;

	std::filesystem::path fonts_config_path;
	std::filesystem::path audio_config_path;
	std::filesystem::path textures_config_path;
	std::filesystem::path characters_config_path;
	std::filesystem::path effects_config_path;

	return resolve_required_config_path(config_registry, "fonts_list", fonts_config_path)
		&& resolve_required_config_path(config_registry, "audio_list", audio_config_path)
		&& resolve_required_config_path(config_registry, "textures_list", textures_config_path)
		&& resolve_required_config_path(config_registry, "character_information", characters_config_path)
		&& resolve_required_config_path(config_registry, "effect_information", effects_config_path)
		&& load_fonts(resource_manager, fonts_config_path)
		&& load_audio(resource_manager, audio_config_path)
		&& load_textures(resource_manager, renderer, textures_config_path)
		&& load_character_animations(characters_config_path, build_atlas)
		&& load_effects(effects_config_path, build_atlas);
}
}
