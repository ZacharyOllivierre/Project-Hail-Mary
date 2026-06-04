#pragma once
#include "../resource_sub_manager.h"
#include "texture_loader.h"

#include <SDL.h>

#include <string>
#include <string_view>
#include <unordered_map>

using TexturePool = std::unordered_map<std::string, TexturePtr>;

class TextureManager : public ResourceSubManager
{
public:
	bool store_texture(const std::string& key, TexturePtr texture);
	SDL_Texture* find_texture(const std::string_view& key) const;

	void clear() override;
	size_t resource_count() const override;

private:
	TexturePool _texture_pool;
};
