#include "scene_manager.h"

#include "../effects/runtime/effect_manager.h"

namespace engine::scene
{

SceneManager::~SceneManager()
{
	shutdown();
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
		detach_from_scene(_current_scene);
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

void SceneManager::attach_to_scene(Scene* scene) noexcept
{
	if (!scene)
		return;

	engine::effects::EffectManager::instance()->bind_active_scene(*scene);
}

void SceneManager::detach_from_scene(Scene* scene) noexcept
{
	if (!scene)
		return;

	engine::effects::EffectManager::instance()->unbind_active_scene(*scene);
}
}
