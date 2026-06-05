#include "test2_obj.h"

#include <iostream>

// Problem
// This shared_ptr keeps TestObj alive even after Scene removes it.

Test2Obj::Test2Obj() : GameObject(DepthLayer::Item)
{
    _test = ResourceManager::instance()->find_texture("test2");
    GameObject::set_position({100, 100});
    GameObject::set_size({300, 300});
    std::cout << "Test2Obj()" << std::endl;
}

Test2Obj::~Test2Obj()
{
    std::cout << "~Test2Obj()" << std::endl;
}

void Test2Obj::submit_render_commands(std::vector<RenderCommand> &out_commands) const
{
    RenderCommand com;
    com.world_rect = GameObject::world_rect();
    com.texture = _test;

    out_commands.push_back(std::move(com));
}
