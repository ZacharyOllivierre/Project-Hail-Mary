#include "resource_manager.h"
#include "../io/path_manager.h"

#include <iostream>

ResourceManager::ResourceManager()
{
}

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

	if (!_texture_manager.load_texture(renderer, "test", PathManager::instance()->assets() / "textures/map.png"))
	{
		std::cout << "Load texture failed." << std::endl;
	}
	else
	{
		std::cout << "Texture loaded." << std::endl;
	}
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

TextureManager& ResourceManager::texture_manager()
{
	return _texture_manager;
}

const TextureManager& ResourceManager::texture_manager() const
{
	return _texture_manager;
}

FontManager& ResourceManager::font_manager()
{
	return _font_manager;
}

const FontManager& ResourceManager::font_manager() const
{
	return _font_manager;
}

AudioManager& ResourceManager::audio_manager()
{
	return _audio_manager;
}

const AudioManager& ResourceManager::audio_manager() const
{
	return _audio_manager;
}