#pragma once

#include <SDL.h>
#include <type_traits>
#include <utility>
#include <vector>

#include "scene.h"
#include "../tools/singleton.h"
#include "scene_factory.h"

class SceneManager : public Singleton<SceneManager>
{
	friend class Singleton<SceneManager>;

private:
	SceneManager() = default;
	~SceneManager();

public:
	void on_update(double delta);
	void on_render(SDL_Renderer* renderer);
	void on_input(
		const InputSnapshot& input,
		const std::vector<InputEvent>& events
	);

	void shutdown();

	template<typename T, typename... Args>
	void switch_to(Args&&... args);

	void reset_current_scene();

	template<typename T>
	bool destroy_scene();

	template<typename T>
	bool reset_scene();

private:
	Scene* _current_scene = nullptr;
	SceneFactory _scene_factory;
};

template<typename T, typename... Args>
void SceneManager::switch_to(Args&&... args)
{
	static_assert(std::is_base_of_v<Scene, T>, "T must derive from Scene");

	T* next_scene = _scene_factory.get_scene<T>(std::forward<Args>(args)...);

	if (!next_scene || _current_scene == next_scene)
		return;

	if (_current_scene)
		_current_scene->on_exit();

	_current_scene = next_scene;
	_current_scene->on_enter();
}

template<typename T>
bool SceneManager::destroy_scene()
{
	static_assert(std::is_base_of_v<Scene, T>, "T must derive from Scene");

	T* target_scene = _scene_factory.try_find_scene<T>();

	if (!target_scene)
		return false;

	if (_current_scene == target_scene)
	{
		_current_scene->on_exit();
		_current_scene = nullptr;
	}

	return _scene_factory.destroy_scene<T>();
}

template<typename T>
bool SceneManager::reset_scene()
{
	static_assert(std::is_base_of_v<Scene, T>, "T must derive from Scene");

	T* target_scene = _scene_factory.try_find_scene<T>();

	if (!target_scene)
		return false;

	target_scene->reset();
	return true;
}
