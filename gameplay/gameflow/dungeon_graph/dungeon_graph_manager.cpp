#include "dungeon_graph_manager.h"

#include <random>
#include <iostream>

// Generates main rooms, classifies them, creates side rooms with
LevelGraph DungeonGraphManager::generate(const LevelConfig &config)
{
    LevelGraph level_graph;

    generate_main_rooms(level_graph, config);
    classify_main_rooms(level_graph, config);

    generate_side_rooms(level_graph, config);

    _dungeon_graph.levels.push_back(level_graph);

    debug_print_config(config);
    debug_print_level(level_graph);

    return level_graph;
}

// Generates main rooms all with type encounter and depth
void DungeonGraphManager::generate_main_rooms(LevelGraph &graph, const LevelConfig &config)
{
    graph.layers.clear();
    graph.layers.reserve(config.main_room_count);

    for (int depth = 0; depth < config.main_room_count; depth++)
    {
        RoomLayer layer;
        layer.depth = depth;

        RoomNode room;
        room.is_main_path = true;
        room.main_type = MainRoomType::Encounter;
        room.side_type = SideRoomType::Count;

        layer.rooms.push_back(room);
        graph.layers.push_back(layer);
    }
}

void DungeonGraphManager::classify_main_rooms(LevelGraph &graph, const LevelConfig &config)
{
    // All are encounter at this point set begginging and end
    graph.layers.front().rooms.front().main_type = MainRoomType::Start;

    const MainRoomType endRoomType =
        config.mini_boss_level ? MainRoomType::MiniBoss : MainRoomType::Boss;

    graph.layers.back().rooms.front().main_type = endRoomType;
}

bool DungeonGraphManager::eligible_for_side_room(MainRoomType type)
{
    // Check for false condition
    switch (type)
    {
    case MainRoomType::Start:
        return false;
    case MainRoomType::MiniBoss:
        return false;
    case MainRoomType::Boss:
        return false;

    default:
        return true;
    }
    return true;
}

void DungeonGraphManager::generate_side_rooms(LevelGraph &graph, const LevelConfig &config)
{
    std::mt19937 rng(config.seed);

    std::uniform_real_distribution<float> chance(0.0f, 1.0f);
    std::uniform_int_distribution<int> room_count(1, config.side_room_config.side_room_max);

    std::discrete_distribution<int> side_room_type(
        config.side_room_config.weights.begin(),
        config.side_room_config.weights.end());

    for (int layer = 0; layer < graph.layers.size(); layer++)
    {
        // Dont generate rooms at start or boss/miniboss
        if (!eligible_for_side_room(graph.layers[layer].rooms.front().main_type))
            continue;

        // Does this eligible layer have side rooms
        if (chance(rng) >= config.side_room_config.side_room_chance)
            continue;

        if (config.side_room_config.side_room_max <= 0)
            return;

        // Roll for number of side rooms
        int count = room_count(rng);

        for (int i = 0; i < count; i++)
        {
            // Roll for type
            SideRoomType type = static_cast<SideRoomType>(side_room_type(rng));

            RoomNode room;
            room.is_main_path = false;
            room.main_type = MainRoomType::Count;
            room.side_type = type;

            // Main room always at index 0 side rooms after it
            graph.layers[layer].rooms.push_back(room);
        }
    }
}

const char *DungeonGraphManager::main_room_type_to_string(MainRoomType type)
{
    switch (type)
    {
    case MainRoomType::Start:
        return "Start";
    case MainRoomType::Encounter:
        return "Encounter";
    case MainRoomType::MiniBoss:
        return "MiniBoss";
    case MainRoomType::Boss:
        return "Boss";
    case MainRoomType::Count:
        return "None";
    }

    return "Unknown";
}

const char *DungeonGraphManager::side_room_type_to_string(SideRoomType type)
{
    switch (type)
    {
    case SideRoomType::Shop:
        return "Shop";
    case SideRoomType::Gift:
        return "Gift";
    case SideRoomType::Encounter:
        return "Encounter";
    case SideRoomType::Count:
        return "None";
    }

    return "Unknown";
}

void DungeonGraphManager::debug_print_config(const LevelConfig &config)
{
    std::cout << "========== Level Config ==========\n";
    std::cout << "Main Room Count: " << config.main_room_count << '\n';
    std::cout << "Mini Boss Level: " << std::boolalpha << config.mini_boss_level << '\n';
    std::cout << "Seed: " << config.seed << '\n';

    std::cout << "\n--- Side Room Config ---\n";
    std::cout << "Side Room Chance: " << config.side_room_config.side_room_chance * 100.0f << "%\n";
    std::cout << "Max Side Rooms: " << config.side_room_config.side_room_max << '\n';
    std::cout << "Side Room Weights:\n";
    for (int i = 0; i < static_cast<int>(SideRoomType::Count); ++i)
    {
        auto type = static_cast<SideRoomType>(i);
        std::cout << "  " << side_room_type_to_string(type) << ": "
                  << config.side_room_config.weights[i] << '\n';
    }
    std::cout << "==================================\n\n";
}

void DungeonGraphManager::debug_print_level(const LevelGraph &graph)
{
    std::cout << "========== Level Graph ==========\n";
    for (const RoomLayer &layer : graph.layers)
    {
        std::cout << "Layer " << layer.depth
                  << " (" << layer.rooms.size() << " rooms)\n";

        for (int i = 0; i < layer.rooms.size(); i++)
        {
            const RoomNode &room = layer.rooms[i];
            std::cout << "  Room " << i;
            if (room.is_main_path)
            {
                std::cout << " MAIN " << main_room_type_to_string(room.main_type);
            }
            else
            {
                std::cout << " SIDE " << side_room_type_to_string(room.side_type);
            }
            std::cout << '\n';
        }
    }
    std::cout << "=================================\n";
}

void DungeonGraphManager::debug_print_dungeon()
{
    std::cout << "========== Dungeon Graph ==========\n";
    std::cout << "Levels: " << _dungeon_graph.levels.size() << "\n\n";

    for (size_t i = 0; i < _dungeon_graph.levels.size(); ++i)
    {
        std::cout << "########## Level " << i << " ##########\n";

        debug_print_level(_dungeon_graph.levels[i]);
        std::cout << '\n';
    }
    std::cout << "===================================\n";
}