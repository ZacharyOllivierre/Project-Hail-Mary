#include "scene_manager.h"

namespace engine::scene
{

SceneManager::~SceneManager()
{
    if (_current_scene)
    {
        _current_scene->on_exit();
        _current_scene = nullptr;
    }
}

void SceneManager::on_update(double delta)
{
    if (_current_scene)
        _current_scene->on_update(delta);
}

void SceneManager::on_render(SDL_Renderer* renderer)
{
    if (_current_scene)
        _current_scene->on_render(renderer);
}

//imgui debug
void SceneManager::on_imgui()
{
    if (_current_scene)
        _current_scene->on_imgui();
}

void SceneManager::on_input(
    const engine::input::InputSnapshot& input,
    const std::vector<engine::input::InputEvent>& events
)
{
    if (_current_scene)
        _current_scene->on_input(input, events);
}

void SceneManager::shutdown()
{
    if (_current_scene)
    {
        _current_scene->on_exit();
        _current_scene = nullptr;
    }

    _scene_factory.destroy_all_scene();
}

void SceneManager::reset_current_scene()
{
    if (_current_scene)
        _current_scene->reset();
}
}
