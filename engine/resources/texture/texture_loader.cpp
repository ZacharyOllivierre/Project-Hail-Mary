#include "texture_loader.h"

#include <iostream>

void TextureDeleter::operator()(SDL_Texture* texture) const
{
	if (texture)
		SDL_DestroyTexture(texture);
}

TextureLoadResult TextureLoader::load_texture(
	SDL_Renderer* renderer,
	const SurfaceLoadResult& surface_result
) const
{
	TextureLoadResult result;
	result._asset_key = surface_result._asset_key;
	result._frame_path = surface_result._frame_path;
	result._frame_index = surface_result._frame_index;

	if (!renderer)
	{
		std::cout << "Load texture failed: renderer is null: "
			<< surface_result._asset_key << std::endl;
		return result;
	}

	if (!surface_result._success || !surface_result._surface)
	{
		std::cout << "Load texture failed: surface is invalid: "
			<< surface_result._frame_path << std::endl;
		return result;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(
		renderer,
		surface_result._surface.get()
	);

	if (!texture)
	{
		std::cout << "Load texture failed: " << surface_result._frame_path
			<< ", reason: " << SDL_GetError() << std::endl;
		return result;
	}

	result._texture.reset(texture);
	result._success = true;
	return result;
}

