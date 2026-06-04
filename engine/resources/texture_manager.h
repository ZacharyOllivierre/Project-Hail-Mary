#pragma once
#include <SDL_image.h>
#include <unordered_map>

typedef std::unordered_map<std::string, SDL_Texture *> TexturePool;

class TextureManager
{
public:
    bool init();
    SDL_Texture *getTexture();
    bool load_texture(SDL_Renderer *renderer, ResourceKey key)

        ~TextureManager();

private:
    TexturePool _texture_pool;
};