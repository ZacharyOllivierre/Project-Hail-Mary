#pragma once

#include <vector>
#include <cstdlib>

using std::vector;

enum class RoomType
{
    Start,
    Normal,
    Boss
};

struct RoomNode
{
    RoomType type = RoomType::Normal;
    bool isMainPath = false;
    int depth = 0;

    vector<RoomNode *> connections;
};

struct DungeonGraph
{
    vector<RoomNode> rooms;
};

// todo change magic numbers
// todo add option branch length
struct DungeonConfig
{
    int mainPathLength = 10;
    int branchChancePercent = 30;
    int maxBranches = 4;

    unsigned int seed = 0;
};

// Generates room graph for dungeon, branching paths and room connectivity before map generation
// todo add error throws
// to do add debug print
class DungeonGraphGenerator
{
public:
    DungeonGraph generate(const DungeonConfig &config);

private:
    RoomNode *createRoom(DungeonGraph &graph);
    void connect(RoomNode *roomA, RoomNode *roomB);

    void generateMainPath(DungeonGraph &graph, const DungeonConfig &config);
    void generateBranches(DungeonGraph &graph, const DungeonConfig &config);
};