#include "resource_manager.h"

#include "atlas/atlas_manager.h"
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
		return false;

	_renderer = renderer;

	if (!PathManager::instance()->init())
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "PathManager init fail");
		return false;
	}

	if (!_texture_manager.load_texture(
		renderer,
		"test",
		PathManager::instance()->assets() / "textures/map.png"))
	{
		std::cout << "Load texture failed." << std::endl;
	}
	else
	{
		std::cout << "Texture loaded." << std::endl;
	}

	if (!_texture_manager.load_texture(
		renderer,
		"test2",
		PathManager::instance()->assets() / "textures/test.png"))
	{
		std::cout << "Load texture failed." << std::endl;
	}
	else
	{
		std::cout << "Texture loaded." << std::endl;
	}

	return true;
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
