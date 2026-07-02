#include "path_manager.h"

#include <iostream>
#include <string>

bool PathManager::init()
{
    std::optional<std::filesystem::path> root_path =
        find_project_root(std::filesystem::current_path());

    if (!root_path.has_value())
    {
        std::cout << "PathManager init failed: project root not found from "
            << std::filesystem::current_path() << std::endl;
        return false;
    }

    _root = root_path.value();
    return true;
}

bool PathManager::ensure_runtime_dirs() const
{
    try
    {
        std::filesystem::create_directories(player_data());
        std::filesystem::create_directories(saves());
        return true;
    }
    catch (const std::filesystem::filesystem_error&)
    {
        return false;
    }
}


const std::filesystem::path& PathManager::root() const
{
    return _root;
}

std::filesystem::path PathManager::assets() const
{
    return _root / "assets";
}

std::filesystem::path PathManager::configs() const
{
    return assets() / "configs";
}

std::filesystem::path PathManager::fonts() const
{
    return assets() / "fonts";
}

std::filesystem::path PathManager::audio() const
{
    return assets() / "audio";
}

std::filesystem::path PathManager::textures() const
{
    return assets() / "textures";
}

std::filesystem::path PathManager::player_data() const
{
    return _root / "player_data";
}

std::filesystem::path PathManager::saves() const
{
    return player_data() / "saves";
}

std::filesystem::path PathManager::configs_list() const
{
    return assets() / "configs_list.json";
}

std::filesystem::path PathManager::resolve_project_path(const std::filesystem::path& path) const
{
    if (path.is_absolute())
        return path.lexically_normal();

    return (_root / path).lexically_normal();
}

std::filesystem::path PathManager::resolve_asset_path(const std::filesystem::path& path) const
{
    if (path.is_absolute())
        return path.lexically_normal();

    if (path_starts_with(path, "assets"))
        return resolve_project_path(path);

    return (assets() / path).lexically_normal();
}

std::filesystem::path PathManager::resolve_config_path(const std::filesystem::path& path) const
{
    if (path.is_absolute())
        return path.lexically_normal();

    if (path_starts_with(path, "assets"))
        return resolve_project_path(path);

    if (path_starts_with(path, "configs"))
        return resolve_asset_path(path);

    return (configs() / path).lexically_normal();
}

bool PathManager::path_starts_with(
    const std::filesystem::path& path,
    const std::string& first_part
) const
{
    std::filesystem::path::iterator iterator = path.begin();
    if (iterator == path.end())
        return false;

    return iterator->string() == first_part;
}

std::optional<std::filesystem::path> PathManager::find_project_root(const std::filesystem::path& start_path) const
{
    constexpr int MAX_SEARCH_DEPTH = 8;

    try
    {
        std::filesystem::path current = std::filesystem::absolute(start_path);
        if (!std::filesystem::is_directory(current))
            current = current.parent_path();

        std::optional<std::filesystem::path> magic_root;
        for (int depth = 0; depth < MAX_SEARCH_DEPTH; ++depth)
        {
            const std::filesystem::path assets_dir_path = current / "assets";
            const std::filesystem::path assets_magic_file_path = assets_dir_path / "._root";
            const std::filesystem::path assets_structure_path = assets_dir_path / "configs_list.json";

            const bool assets_is_dir = std::filesystem::is_directory(assets_dir_path);
            const bool has_assets_magic_file = std::filesystem::exists(assets_magic_file_path);
            const bool has_assets_structure = std::filesystem::exists(assets_structure_path);

            if (assets_is_dir && has_assets_structure)
                return current;

            if (assets_is_dir && has_assets_magic_file && !magic_root.has_value())
                magic_root = current;

            if (current == current.root_path())
                break;

            current = current.parent_path();
        }

        if (magic_root.has_value())
            return magic_root;
    }
    catch (const std::filesystem::filesystem_error&)
    {
        return std::nullopt;
    }

    return std::nullopt;
}
