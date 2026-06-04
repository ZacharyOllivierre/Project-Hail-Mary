#include "resource_manager.h"

ResourceManager::ResourceManager()
	: _atlas_manager(_texture_manager)
{
}

bool ResourceManager::load_atlas(
	SDL_Renderer* renderer,
	const AtlasLoadRequest& request
)
{
	return _atlas_manager.load_atlas(renderer, request);
}

bool ResourceManager::load_atlases(
	SDL_Renderer* renderer,
	const std::vector<AtlasLoadRequest>& requests
)
{
	return _atlas_manager.load_atlases(renderer, requests);
}

bool ResourceManager::load_font(
	const std::string& key,
	const std::filesystem::path& file_path,
	int point_size
)
{
	return _font_manager.load_font(key, file_path, point_size);
}

Atlas* ResourceManager::find_atlas(const std::string_view& key) const
{
	return _atlas_manager.find_atlas(key);
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

SDL_Texture* ResourceManager::find_texture(const std::string_view& key) const
{
	return _texture_manager.find_texture(key);
}

AtlasManager& ResourceManager::atlas_manager()
{
	return _atlas_manager;
}

const AtlasManager& ResourceManager::atlas_manager() const
{
	return _atlas_manager;
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

void ResourceManager::clear()
{
	_atlas_manager.clear();
	_texture_manager.clear();
	_font_manager.clear();
	_audio_manager.clear();
}

size_t ResourceManager::resource_count() const
{
	return _atlas_manager.resource_count()
		+ _texture_manager.resource_count()
		+ _font_manager.resource_count()
		+ _audio_manager.resource_count();
}
