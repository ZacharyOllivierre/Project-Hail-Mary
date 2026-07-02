#include "resource_manager.h"

#include "atlas/atlas_manager.h"
#include "resource_bootstrapper.h"
#include "../io/path_manager.h"

#include <iostream>
#include <memory>

ResourceManager::ResourceManager()
	: _atlas_manager(std::make_unique<AtlasManager>())
{
}

ResourceManager::~ResourceManager() = default;

bool ResourceManager::init(SDL_Renderer* renderer)
{
	if (!renderer)
	{
		std::cout << "ResourceManager init failed: renderer is null." << std::endl;
		return false;
	}

	_renderer = renderer;

	if (!PathManager::instance()->init())
	{
		std::cout << "ResourceManager init failed: PathManager init fail." << std::endl;
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "PathManager init fail");
		return false;
	}

	if (!ResourceBootstrapper::bootstrap(*this, renderer))
	{
		std::cout << "ResourceManager init failed: resource bootstrap failed."
			<< std::endl;
		return false;
	}

	return true;
}

bool ResourceManager::load_font(
	const std::string& key,
	const std::filesystem::path& file_path,
	int point_size
)
{
	return _font_manager.load_font(key, file_path, point_size);
}

bool ResourceManager::load_sound(
	const std::string& key,
	const std::filesystem::path& file_path
)
{
	return _audio_manager.load_sound(key, file_path);
}

bool ResourceManager::load_music(
	const std::string& key,
	const std::filesystem::path& file_path
)
{
	return _audio_manager.load_music(key, file_path);
}

bool ResourceManager::load_texture(
	SDL_Renderer* renderer,
	const std::string& key,
	const std::filesystem::path& file_path
)
{
	return _texture_manager.load_texture(renderer, key, file_path);
}

TTF_Font* ResourceManager::find_font(const std::string_view& key) const
{
	return _font_manager.find_font(key);
}

Mix_Chunk* ResourceManager::find_sound(const std::string_view& key) const
{
	return _audio_manager.find_sound(key);
}

Mix_Music* ResourceManager::find_music(const std::string_view& key) const
{
	return _audio_manager.find_music(key);
}

SDL_Texture* ResourceManager::find_texture(const std::string_view& key)
{
	return _texture_manager.find_texture(key);
}

const Atlas* ResourceManager::find_atlas(const std::string_view& key) const
{
	return _atlas_manager ? _atlas_manager->find_atlas(key) : nullptr;
}

const Atlas* ResourceManager::build_atlas(
	SDL_Renderer* renderer,
	const AtlasLoadRequest& request
)
{
	if (!renderer)
		return nullptr;

	if (!_atlas_manager->load_atlas(renderer, request, _texture_manager))
		return nullptr;

	return _atlas_manager->find_atlas(request.atlas_key);
}
