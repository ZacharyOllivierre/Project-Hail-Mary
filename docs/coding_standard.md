# C++ Coding Standard

## Naming Conventions

- Use `NameName` (PascalCase) for classes, enums, and structs.
- Use `_name_name` for member variables.
- Use `name_name` for local variables.
- Use `name_name` for function parameters.
- Use `name_name` for functions and member functions.
- Use `name_name` for file names.

Good example:

```cpp
class PathManager
{
private:
    std::filesystem::path _root_path;
};

struct PlayerData
{
    int hp;
};

enum class SceneType
{
    MainMenu,
    BattleScene
};

void set_player_data(const PlayerData& player_data);
```

Bad example:

```cpp
class path_manager {};
struct playerData {};
enum class scene_type {};

std::filesystem::path RootPath;
void SetPlayerData(const PlayerData& playerData);
```

## `auto` Usage Guidelines

- When creating variables of project-defined types, prefer writing the type explicitly and avoid using `auto` whenever possible.
- Use `auto` carefully only when the right-hand type is immediately obvious or writing the explicit type would clearly reduce readability.

Good example:

```cpp
PathManager* path_manager = PathManager::instance();
std::filesystem::path asset_path = root_path / "assets";
std::optional<std::filesystem::path> root_path = find_project_root(start_path);
```

Bad example:

```cpp
auto path_manager = PathManager::instance();
auto asset_path = root_path / "assets";
auto root_path = find_project_root(start_path);
```

## Code Completeness Requirements

- Ensure that the source code is in a compilable state for every commit.
- For unfinished implementations, write a `TODO` placeholder first.

Good example:

```cpp
std::optional<std::filesystem::path> PathManager::find_project_root(const std::filesystem::path& start_path) const
{
    // TODO: implement project root detection.
    (void)start_path;
    return std::nullopt;
}
```

Bad example:

```cpp
std::optional<std::filesystem::path> PathManager::find_project_root(const std::filesystem::path& start_path) const
{
    std::filesystem::path current = std::filesystem::absolute(start_path)
}
```

Do not use `using namespace std;` in header files.
Avoid using `using namespace std;` in source files unless the scope is very small.