#include "atlas_manager.h"

#include "../texture/texture_manager.h"
#include "../../tools/logger.h"

namespace engine::resources
{

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
		ENGINE_LOG_WARN("resource","Load atlas failed: renderer is empty.");
		return false;
	}

	if (request.atlas_key.empty())
	{
		ENGINE_LOG_WARN("resource","Load atlas failed: atlas key is empty.");
		return false;
	}

	if (request.frame_paths.empty())
	{
		ENGINE_LOG_WARN("resource","Load atlas failed: frame path list is empty: "
			<< request.atlas_key);
		return false;
	}

	if (_atlas_pool.contains(request.atlas_key))
		return true;

	Atlas atlas(request.atlas_key);

	for (size_t frame_index = 0; frame_index < request.frame_paths.size(); ++frame_index)
	{
		const std::filesystem::path& frame_path = request.frame_paths[frame_index];
		const std::string texture_key =
			request.atlas_key + "#" + std::to_string(frame_index);

		SDL_Texture* texture = texture_manager.find_texture(texture_key);
		if (!texture)
		{
			if (!texture_manager.load_texture(renderer, texture_key, frame_path))
			{
				ENGINE_LOG_WARN("resource","Load atlas frame texture failed: "
					<< texture_key << " <- " << frame_path.string());
				return false;
			}

			texture = texture_manager.find_texture(texture_key);
		}

		if (!texture)
		{
			ENGINE_LOG_WARN("resource","Resolve atlas frame texture failed: " << texture_key);
			return false;
		}

		if (!atlas.add_frame(frame_path, texture))
		{
			ENGINE_LOG_WARN("resource","Add atlas frame failed: " << frame_path.string());
			return false;
		}
	}

	_atlas_pool.emplace(request.atlas_key, std::move(atlas));
	return true;
}
}
