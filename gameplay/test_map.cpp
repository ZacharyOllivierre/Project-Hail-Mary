#include "test_map.h"

#include "../engine/core/render/render_command.h"

#include <iostream>

TestMap::TestMap() : GameObject(DepthLayer::Item)
{
    _texture = ResourceManager::instance()->find_texture("test2");
    set_position({100, 100});
    set_size({3000, 3000});
    std::cout << "TestMap()" << std::endl;
}

TestMap::~TestMap()
{
    std::cout << "~TestMap()" << std::endl;
}

void TestMap::submit_render_commands(std::vector<RenderCommand> &out_commands) const
{
    RenderCommand command;
    command.command_rect = world_rect();
    command.texture = _texture;

    out_commands.push_back(std::move(command));
}
