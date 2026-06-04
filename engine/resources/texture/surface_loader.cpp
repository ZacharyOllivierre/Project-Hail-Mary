#include "surface_loader.h"

#include <SDL_image.h>

#include <iostream>

void SurfaceDeleter::operator()(SDL_Surface* surface) const
{
	if (surface)
		SDL_FreeSurface(surface);
}

SurfaceLoadResult SurfaceLoader::load_surface(const SurfaceLoadRequest& request) const
{
	SurfaceLoadResult result;
	result._asset_key = request._asset_key;
	result._frame_path = request._frame_path;
	result._frame_index = request._frame_index;

	if (request._asset_key.empty())
	{
		std::cout << "Load surface failed: asset key is empty." << std::endl;
		return result;
	}

	if (request._frame_path.empty())
	{
		std::cout << "Load surface failed: frame path is empty: "
			<< request._asset_key << std::endl;
		return result;
	}

	SDL_Surface* surface = IMG_Load(request._frame_path.string().c_str());
	if (!surface)
	{
		std::cout << "Load surface failed: " << request._frame_path
			<< ", reason: " << IMG_GetError() << std::endl;
		return result;
	}

	result._surface.reset(surface);
	result._success = true;
	return result;
}

