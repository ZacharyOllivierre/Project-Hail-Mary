#include "dungeon_graph_generator.h"

// Generates dungeon graph with main and side paths
DungeonGraph DungeonGraphGenerator::generate(const DungeonConfig &config)
{
    std::srand(config.seed);

    DungeonGraph graph;

    generateMainPath(graph, config);
    generateBranches(graph, config);

    return graph;
}

// Creates room, adds it to dungeon graph's room list
RoomNode *DungeonGraphGenerator::createRoom(DungeonGraph &graph)
{
    RoomNode room;
    graph.rooms.push_back(room);

    return &graph.rooms.back();
}

// Adds roomB to roomA's connections and vice versa
void DungeonGraphGenerator::connect(RoomNode *roomA, RoomNode *roomB)
{
    if (!roomA || !roomB)
    {
        return;
    }

    // Check for duplicates
    for (RoomNode *connection : roomA->connections)
    {
        if (connection == roomB)
        {
            return;
        }
    }

    roomA->connections.push_back(roomB);
    roomB->connections.push_back(roomA);
}

// Generates main linear main path of n rooms with first as start last as boss
void DungeonGraphGenerator::generateMainPath(DungeonGraph &graph, const DungeonConfig &config)
{
    // Set first room to start
    RoomNode *previous = createRoom(graph);
    previous->type = RoomType::Start;

    previous->isMainPath = true;
    previous->depth = 0;

    // Create n more rooms connecting each to previous
    for (int i = 1; i < config.mainPathLength; i++)
    {
        RoomNode *current = createRoom(graph);
        connect(previous, current);

        current->isMainPath = true;
        current->depth = i;

        previous = current;
    }

    previous->type = RoomType::Boss;
}

// Each room has percent change to generate side room
// Todo fix earlier rooms have higher chance to have side rooms than later ones
void DungeonGraphGenerator::generateBranches(DungeonGraph &graph, const DungeonConfig &config)
{
    int branchCount = 0;
    // Used to iterate only through the origninal rooms in vec
    const int initialSize = (int)graph.rooms.size();

    for (int i = 0; i < initialSize; i++)
    {
        if (branchCount >= config.maxBranches)
        {
            break;
        }

        RoomNode &node = graph.rooms[i];

        // Shouldnt hit continue because looping through initsize
        if (!node.isMainPath)
        {
            continue;
        }

        if (node.type == RoomType::Start || node.type == RoomType::Boss)
        {
            continue;
        }

        bool hasSideRoom = rand() % 100 < config.branchChancePercent;

        if (hasSideRoom)
        {
            RoomNode *sideRoom = createRoom(graph);
            sideRoom->isMainPath = false;
            sideRoom->depth = node.depth + 1;

            connect(&node, sideRoom);
            branchCount++;
        }
    }
}