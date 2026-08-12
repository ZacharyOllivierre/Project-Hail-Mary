#pragma once

#include <vector>

enum class MainRoomType
{
    Start,
    Encounter,
    MiniBoss,
    Boss,
    Count,
};

enum class SideRoomType
{
    Shop,
    Gift,
    Encounter,
    Count,
};

// TODO dont need to split graph with this many structs
// Keep only what becomes a neccessary distinction

// Single room
struct RoomNode
{
    bool is_main_path = false;

    MainRoomType main_type = MainRoomType::Count;
    SideRoomType side_type = SideRoomType::Count;
};

// A set of rooms of a given depth in a level graph
struct RoomLayer
{
    // Main room of layer always at index 0, side rooms after
    std::vector<RoomNode> rooms;
    int depth = 0;
};

// A set of layers to make up one level
struct LevelGraph
{
    std::vector<RoomLayer> layers;
};

// Set of levels to make full dungeon, is created one level at a time
struct DungeonGraph
{
    std::vector<LevelGraph> levels;
};

struct SideRoomConfig
{
    float side_room_chance = 0.7f;
    int side_room_max = 3;

    // Chance for each type to room to be created
    std::array<float, (int)SideRoomType::Count> weights{
        0.2f, // Shop
        0.2f, // Gift
        0.6f  // Encounter
    };
};

struct LevelConfig
{
    // Number of neccessary main rooms of given level
    int main_room_count = 6;

    // Whether last main room will be mini or main boss
    bool mini_boss_level = true;

    SideRoomConfig side_room_config;

    unsigned int seed = 1;
};
