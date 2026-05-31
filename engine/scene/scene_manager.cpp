#include "scene_manager.h"

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

void SceneManager::on_input(
    const InputSnapshot& input,
    const std::vector<InputEvent>& events
)
{
    if (_current_scene)
        _current_scene->on_input(input, events);
}

void SceneManager::reset_current_scene()
{
    if (_current_scene)
        _current_scene->reset();
}



