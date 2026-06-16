#include "atlas_manager.h"

#include "../texture/texture_manager.h"

#include <iostream>

const Atlas* AtlasManager::find_atlas(const std::string_view& key) const
{
	AtlasPool::const_iterator iterator = _atlas_pool.find(std::string(key));
	if (iterator == _atlas_pool.end())
		return nullptr;

	return &iterator->second;
}

bool AtlasManager::load_atlas(
	SDL_Renderer* renderer,
	const AtlasLoadRequest& request,
	TextureManager& texture_manager
)
{
	if (!renderer)
	{
		std::cout << "Load atlas failed: renderer is empty." << std::endl;
		return false;
	}

	if (request._atlas_key.empty())
	{
		std::cout << "Load atlas failed: atlas key is empty." << std::endl;
		return false;
	}

	if (request._frame_paths.empty())
	{
		std::cout << "Load atlas failed: frame path list is empty: "
			<< request._atlas_key << std::endl;
		return false;
	}

	if (_atlas_pool.contains(request._atlas_key))
		return true;

	Atlas atlas(request._atlas_key);

	for (size_t frame_index = 0; frame_index < request._frame_paths.size(); ++frame_index)
	{
		const std::filesystem::path& frame_path = request._frame_paths[frame_index];
		const std::string texture_key =
			request._atlas_key + "#" + std::to_string(frame_index);

		SDL_Texture* texture = texture_manager.find_texture(texture_key);
		if (!texture)
		{
			if (!texture_manager.load_texture(renderer, texture_key, frame_path))
			{
				std::cout << "Load atlas frame texture failed: "
					<< texture_key << " <- " << frame_path.string() << std::endl;
				return false;
			}

			texture = texture_manager.find_texture(texture_key);
		}

		if (!texture)
		{
			std::cout << "Resolve atlas frame texture failed: "
				<< texture_key << std::endl;
			return false;
		}

		if (!atlas.add_frame(frame_path, texture))
		{
			std::cout << "Add atlas frame failed: "
				<< frame_path.string() << std::endl;
			return false;
		}
	}

	_atlas_pool.emplace(request._atlas_key, std::move(atlas));
	return true;
}
