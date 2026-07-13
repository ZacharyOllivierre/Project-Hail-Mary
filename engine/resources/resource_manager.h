 #pragma once
#include "../tools/singleton.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <filesystem>
#include <memory>
#include <string_view>
#include "audio/audio_manager.h"
#include "font/font_manager.h"
#include "resource_types.h"
#include "texture/texture_manager.h"

namespace engine::resources
{

class Atlas;
class AtlasManager;
class ResourceBootstrapper;

class ResourceManager : public engine::tools::Singleton<ResourceManager>
{
	friend engine::tools::Singleton<ResourceManager>;
	friend class ResourceBootstrapper;

public:
	ResourceManager();
	~ResourceManager();

	bool init(SDL_Renderer* renderer);

	bool load_font(
		const std::string& key,
		const std::filesystem::path& file_path,
		int point_size
	);
	bool load_sound(
		const std::string& key,
		const std::filesystem::path& file_path
	);
	bool load_music(
		const std::string& key,
		const std::filesystem::path& file_path
	);
	bool load_texture(
		SDL_Renderer* renderer,
		const std::string& key,
		const std::filesystem::path& file_path
	);

	TTF_Font* find_font(const std::string_view& key) const;
	Mix_Chunk* find_sound(const std::string_view& key) const;
	Mix_Music* find_music(const std::string_view& key) const;
	SDL_Texture* find_texture(const std::string_view& key);
	const Atlas* find_atlas(const std::string_view& key) const;

private:
	const Atlas* build_atlas(SDL_Renderer* renderer, const AtlasLoadRequest& request);

	SDL_Renderer* _renderer=nullptr;

	TextureManager _texture_manager;
	FontManager _font_manager;
	AudioManager _audio_manager;
	std::unique_ptr<AtlasManager> _atlas_manager;
};
}
