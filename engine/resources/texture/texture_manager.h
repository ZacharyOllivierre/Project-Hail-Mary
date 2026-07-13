#pragma once

#include <SDL.h>

#include <string>
#include <filesystem>
#include <string_view>
#include <unordered_map>

namespace engine::resources
{

using TexturePool = std::unordered_map<std::string, SDL_Texture*>;

class TextureManager
{
public:
	bool load_texture(SDL_Renderer* renderer, const std::string& key,std::filesystem::path file_path);

	bool store_texture(const std::string& key, SDL_Texture* texture);
	SDL_Texture* find_texture(const std::string_view& key);

private:
	TexturePool _texture_pool;
};
}
