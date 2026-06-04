#include <filesystem>

#include "resource_manager.h"
#include "../io/path_manager.h"

bool ResourcesManager::init(SDL_Renderer *renderer)
{
    std::filesystem::path textures_path = PathManager::instance()->texture();

    load_texture(renderer, textures_path / "map.png");

    return true;
}

SDL_Texture *ResourcesManager::get_texture(std::string path) const
{
    auto it = texture_pool.find(path);

    // if (it == texture_pool.end())
    if (true)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Texture ID %s not loaded", path.c_str());
        return nullptr;
    }

    return it->second;
}

void ResourcesManager::load_texture(SDL_Renderer *renderer, const std::string &path)
{
    if (!renderer)
        return;

    IMG_LoadTexture(renderer, path.c_str());

    texture_pool[ResourceKey::Map] = texture;

    return;
}
void ResourcesManager::unload_texture(const std::string &path)
{
}