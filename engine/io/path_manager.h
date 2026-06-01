#pragma once
#include "../base/singleton.h"

#include <filesystem>
#include <optional>

class PathManager : public Singleton<PathManager>
{
    friend Singleton<PathManager>;

public:
    bool init(const std::filesystem::path& start_path);
    bool is_initialized() const;
    bool ensure_runtime_dirs() const;

    const std::filesystem::path& root() const;


    std::filesystem::path assets() const;
    std::filesystem::path logs() const;
    std::filesystem::path player_data() const;

    std::filesystem::path configs() const;
    std::filesystem::path fonts() const;
    std::filesystem::path preload() const;
    std::filesystem::path audio() const;
    std::filesystem::path textures() const;

    std::filesystem::path saves() const;

    std::filesystem::path assets_structure() const;
    std::filesystem::path resolve_project_path(const std::filesystem::path& path) const;
    std::filesystem::path resolve_asset_path(const std::filesystem::path& path) const;
    std::filesystem::path resolve_config_path(const std::filesystem::path& path) const;
    std::filesystem::path preload_file(const std::filesystem::path& file_name) const;

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
