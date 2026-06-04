#pragma once

#include "surface_loader.h"

#include <SDL.h>

#include <filesystem>
#include <memory>
#include <string>

struct TextureDeleter
{
	void operator()(SDL_Texture* texture) const;
};

using TexturePtr = std::unique_ptr<SDL_Texture, TextureDeleter>;

struct TextureLoadResult
{
	bool _success = false;
	std::string _asset_key;
	std::filesystem::path _frame_path;
	size_t _frame_index = 0;
	TexturePtr _texture;
};

class TextureLoader
{
public:
	TextureLoadResult load_texture(
		SDL_Renderer* renderer,
		const SurfaceLoadResult& surface_result
	) const;
};

