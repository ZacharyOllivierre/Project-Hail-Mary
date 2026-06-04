#pragma once

#include <SDL.h>

#include <string>
#include <string_view>
#include <unordered_map>

using TexturePool = std::unordered_map<std::string, SDL_Texture*>;

class TextureManager
{
public:
	bool store_texture(const std::string& key, SDL_Texture* texture);
	SDL_Texture* find_texture(const std::string_view& key);

private:
	TexturePool _texture_pool;
};
