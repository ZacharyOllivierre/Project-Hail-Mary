#include "atlas_manager.h"

#include "atlas_builder.h"
#include "../texture/surface_loader.h"
#include "../texture/texture_loader.h"
#include "../texture/texture_manager.h"

#include <algorithm>
#include <cctype>
#include <iostream>

AtlasManager::AtlasManager(TextureManager& texture_manager)
	: _texture_manager(texture_manager)
{
}

bool AtlasManager::load_atlas(SDL_Renderer* renderer, const AtlasLoadRequest& request)
{
	if (!renderer)
	{
		std::cout << "Load atlas failed: renderer is null: "
			<< request._atlas_key << std::endl;
		return false;
	}

	if (request._atlas_key.empty())
	{
		std::cout << "Load atlas failed: atlas key is empty." << std::endl;
		return false;
	}

	if (_atlas_pool.contains(request._atlas_key))
		return true;

	std::vector<std::filesystem::path> frame_paths;
	if (!collect_frame_paths(request, frame_paths))
		return false;

	SurfaceLoader surface_loader;
	TextureLoader texture_loader;
	std::vector<TextureLoadResult> texture_results;
	texture_results.reserve(frame_paths.size());

	for (size_t index = 0; index < frame_paths.size(); ++index)
	{
		SurfaceLoadRequest surface_request;
		surface_request._asset_key = request._atlas_key;
		surface_request._frame_path = frame_paths[index];
		surface_request._frame_index = index;

		SurfaceLoadResult surface_result = surface_loader.load_surface(surface_request);
		if (!surface_result._success)
			return false;

		TextureLoadResult texture_result =
			texture_loader.load_texture(renderer, surface_result);
		if (!texture_result._success)
			return false;

		texture_results.push_back(std::move(texture_result));
	}

	std::unique_ptr<Atlas> atlas = std::make_unique<Atlas>(request._atlas_key);
	AtlasBuilder atlas_builder;
	if (!atlas_builder.build_atlas(request, texture_results, *atlas))
		return false;

	for (size_t index = 0; index < texture_results.size(); ++index)
	{
		std::string texture_key = make_texture_key(request._atlas_key, index);
		if (!_texture_manager.store_texture(
			texture_key,
			std::move(texture_results[index]._texture)))
		{
			return false;
		}
	}

	_atlas_pool.emplace(request._atlas_key, std::move(atlas));
	return true;
}

bool AtlasManager::load_atlases(
	SDL_Renderer* renderer,
	const std::vector<AtlasLoadRequest>& requests
)
{
	if (!renderer)
	{
		std::cout << "Load atlases failed: renderer is null." << std::endl;
		return false;
	}

	for (const AtlasLoadRequest& request : requests)
	{
		if (!load_atlas(renderer, request))
			return false;
	}

	return true;
}

Atlas* AtlasManager::find_atlas(const std::string_view& key) const
{
	AtlasPool::const_iterator iterator = _atlas_pool.find(std::string(key));
	if (iterator == _atlas_pool.end())
		return nullptr;

	return iterator->second.get();
}

void AtlasManager::clear()
{
	_atlas_pool.clear();
}

size_t AtlasManager::resource_count() const
{
	return _atlas_pool.size();
}

bool AtlasManager::collect_frame_paths(
	const AtlasLoadRequest& request,
	std::vector<std::filesystem::path>& frame_paths
) const
{
	frame_paths.clear();

	if (request._directory_path.empty())
	{
		std::cout << "Collect frame paths failed: directory path is empty: "
			<< request._atlas_key << std::endl;
		return false;
	}

	if (!std::filesystem::is_directory(request._directory_path))
	{
		std::cout << "Collect frame paths failed: directory does not exist: "
			<< request._directory_path << std::endl;
		return false;
	}

	for (const std::filesystem::directory_entry& entry :
		std::filesystem::directory_iterator(request._directory_path))
	{
		if (!entry.is_regular_file())
			continue;

		std::string extension = entry.path().extension().string();
		for (char& character : extension)
			character = static_cast<char>(
				std::tolower(static_cast<unsigned char>(character))
			);

		if (extension != ".png")
			continue;

		frame_paths.push_back(entry.path());
	}

	std::sort(
		frame_paths.begin(),
		frame_paths.end(),
		[](const std::filesystem::path& lhs, const std::filesystem::path& rhs)
		{
			return lhs.filename().string() < rhs.filename().string();
		}
	);

	if (frame_paths.size() != request._frame_count)
	{
		std::cout << "Collect frame paths failed: frame count mismatch: "
			<< request._atlas_key << ", expected " << request._frame_count
			<< ", actual " << frame_paths.size() << std::endl;
		return false;
	}

	return true;
}

std::string AtlasManager::make_texture_key(
	const std::string& atlas_key,
	size_t frame_index
) const
{
	return atlas_key + "#" + std::to_string(frame_index);
}
