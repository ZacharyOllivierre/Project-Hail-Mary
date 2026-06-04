#pragma once
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

	bool init(SDL_Renderer* renderer);

	TTF_Font* find_font(const std::string_view& key) const;
	Mix_Chunk* find_sound(const std::string_view& key) const;
	Mix_Music* find_music(const std::string_view& key) const;
	SDL_Texture* find_texture(const std::string_view& key);

	TextureManager& texture_manager();
	const TextureManager& texture_manager() const;
	FontManager& font_manager();
	const FontManager& font_manager() const;
	AudioManager& audio_manager();
	const AudioManager& audio_manager() const;

private:
	SDL_Renderer* _renderer=nullptr;

	TextureManager _texture_manager;
	FontManager _font_manager;
	AudioManager _audio_manager;
};
