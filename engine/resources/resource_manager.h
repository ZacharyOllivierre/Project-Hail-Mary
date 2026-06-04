#pragma once
#include <SDL_image.h>
#include <unordered_map>

#include "../tools/singleton.h"

typedef std::unordered_map<std::string, SDL_Texture *> TexturePool;

class ResourceManager
{
public:
    ResourceManager();
    bool init();

    SDL_Texture *get_texture(const std::string &path) const;

private:
    void load_texture(const std::string &path);
    void unload_texture(const std::string &path);

private:
    TexturePool _texture_pool;
};