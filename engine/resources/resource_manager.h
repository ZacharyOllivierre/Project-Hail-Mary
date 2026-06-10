#pragma once
#include "../tools/singleton.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <memory>
#include <string_view>
#include "audio/audio_manager.h"
#include "font/font_manager.h"
#include "resource_types.h"
#include "texture/texture_manager.h"

class Atlas;
class AtlasManager;
class ResourceBootstrapper;

class ResourceManager : public Singleton<ResourceManager>
{
	friend Singleton<ResourceManager>;
	friend class ResourceBootstrapper;

public:
	ResourceManager();
	~ResourceManager();

	bool init(SDL_Renderer* renderer);

	TTF_Font* find_font(const std::string_view& key) const;
	Mix_Chunk* find_sound(const std::string_view& key) const;
	Mix_Music* find_music(const std::string_view& key) const;
	SDL_Texture* find_texture(const std::string_view& key);

private:
	const Atlas* build_atlas(SDL_Renderer* renderer, const AtlasLoadRequest& request);

	SDL_Renderer* _renderer=nullptr;

	TextureManager _texture_manager;
	FontManager _font_manager;
	AudioManager _audio_manager;
	std::unique_ptr<AtlasManager> _atlas_manager;
};
