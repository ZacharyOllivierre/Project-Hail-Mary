#pragma once

#include <SDL.h>

class ResourceManager;

class ResourceBootstrapper
{
public:
	static bool bootstrap(ResourceManager& resource_manager, SDL_Renderer* renderer);
};
