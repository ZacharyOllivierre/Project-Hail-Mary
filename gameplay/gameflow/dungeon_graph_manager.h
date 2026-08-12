#pragma once

#include "dungeon_graph_types.h"

class DungeonGraphManager
{
public:
    LevelGraph generate(const LevelConfig &config = LevelConfig{});

    inline DungeonGraph dungeon_graph() { return _dungeon_graph; }

private:
    void generate_main_rooms(LevelGraph &graph, const LevelConfig &config);
    void classify_main_rooms(LevelGraph &graph, const LevelConfig &config);

    bool eligible_for_side_room(MainRoomType type);
    void generate_side_rooms(LevelGraph &graph, const LevelConfig &config);

    const char *main_room_type_to_string(MainRoomType type);
    const char *side_room_type_to_string(SideRoomType type);
    void debug_print_config(const LevelConfig &config);
    void debug_print_level(const LevelGraph &graph);
    void debug_print_dungeon();

private:
    DungeonGraph _dungeon_graph;
};
