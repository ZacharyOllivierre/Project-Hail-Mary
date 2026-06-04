#pragma once
#include "atlas/atlas_manager.h"
#include "audio/audio_manager.h"
#include "font/font_manager.h"
#include "texture/texture_manager.h"

#include "../tools/singleton.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <filesystem>
#include <string_view>
#include <vector>

class ResourceManager : public Singleton<ResourceManager>
{
	friend Singleton<ResourceManager>;

public:
	ResourceManager();

	bool load_atlas(SDL_Renderer* renderer, const AtlasLoadRequest& request);
	bool load_atlases(
		SDL_Renderer* renderer,
		const std::vector<AtlasLoadRequest>& requests
	);
	bool load_font(
		const std::string& key,
		const std::filesystem::path& file_path,
		int point_size
	);

	Atlas* find_atlas(const std::string_view& key) const;
	TTF_Font* find_font(const std::string_view& key) const;
	Mix_Chunk* find_sound(const std::string_view& key) const;
	Mix_Music* find_music(const std::string_view& key) const;
	SDL_Texture* find_texture(const std::string_view& key) const;

	AtlasManager& atlas_manager();
	const AtlasManager& atlas_manager() const;
	TextureManager& texture_manager();
	const TextureManager& texture_manager() const;
	FontManager& font_manager();
	const FontManager& font_manager() const;
	AudioManager& audio_manager();
	const AudioManager& audio_manager() const;

	void clear();
	size_t resource_count() const;

private:
	TextureManager _texture_manager;
	AtlasManager _atlas_manager;
	FontManager _font_manager;
	AudioManager _audio_manager;
};
