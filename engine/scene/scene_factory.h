#pragma once
#include "scene.h"

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <type_traits>
#include <utility>

//Factory + Registry
class SceneFactory
{
public:
	SceneFactory() = default;
	~SceneFactory();

	SceneFactory(const SceneFactory&) = delete;
	SceneFactory& operator=(const SceneFactory&) = delete;
	SceneFactory(SceneFactory&&) = delete;
	SceneFactory& operator=(SceneFactory&&) = delete;

	template<typename T, typename... Args>
	T* get_scene(Args&&... args);

	template<typename T>
	T* try_find_scene()const;

	template<typename T>
	bool destroy_scene();

	bool destroy_all_scene();

private:
	std::unordered_map<std::type_index, std::unique_ptr<Scene>> _scene_cache;
};

template<typename T, typename... Args>
T* SceneFactory::get_scene(Args&&... args)
{
	static_assert(std::is_base_of_v<Scene, T>, "T must derive from Scene");

	// Scenes are cached by concrete type and reused after the first creation.
	// If a cached scene already exists, new constructor arguments are ignored.
	// To reload a scene, reset the existing instance or destroy it before recreating it.
	const std::type_index cache_key(typeid(T));
	auto iter = _scene_cache.find(cache_key);

	if (iter != _scene_cache.end())
		return static_cast<T*>(iter->second.get());

	auto scene = std::make_unique<T>(std::forward<Args>(args)...);
	T* scene_ptr = scene.get();

	_scene_cache.emplace(cache_key, std::move(scene));

	return scene_ptr;
}

template<typename T>
T* SceneFactory::try_find_scene()const
{
	static_assert(std::is_base_of_v<Scene, T>, "T must derive from Scene");

	std::type_index key(typeid(T));
	auto iter = _scene_cache.find(key);

	if (iter != _scene_cache.end())
		return static_cast<T*>(iter->second.get());

	return nullptr;
}

template<typename T>
bool SceneFactory::destroy_scene()
{
	static_assert(std::is_base_of_v<Scene, T>, "T must derive from Scene");

	std::type_index key(typeid(T));
	auto iter = _scene_cache.find(key);

	if (iter == _scene_cache.end())
		return false;

	_scene_cache.erase(iter);

	return true;
}
