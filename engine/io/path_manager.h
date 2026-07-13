#pragma once
#include "../tools/singleton.h"

#include <filesystem>
#include <optional>

namespace engine::io
{

class PathManager : public ::engine::tools::Singleton<PathManager>
{
    friend ::engine::tools::Singleton<PathManager>;

public:
    bool init();
    bool ensure_runtime_dirs() const;

    const std::filesystem::path& root() const;


    std::filesystem::path assets() const;
    std::filesystem::path player_data() const;

    std::filesystem::path configs() const;
    std::filesystem::path fonts() const;
    std::filesystem::path audio() const;
    std::filesystem::path textures() const;

    std::filesystem::path saves() const;

    std::filesystem::path configs_list() const;

    std::filesystem::path resolve_project_path(const std::filesystem::path& path) const;
    std::filesystem::path resolve_asset_path(const std::filesystem::path& path) const;
    std::filesystem::path resolve_config_path(const std::filesystem::path& path) const;

private:
    PathManager() = default;

    bool path_starts_with(
        const std::filesystem::path& path,
        const std::string& first_part
    ) const;
    std::optional<std::filesystem::path> find_project_root(const std::filesystem::path& start_path) const;

private:
    std::filesystem::path _root;
};
}
