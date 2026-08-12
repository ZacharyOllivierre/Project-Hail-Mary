#pragma once

#include <SDL.h>

#include <array>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "../camera/camera.h"

#include "../core/depth_layer.h"
#include "../core/game_object.h"
#include "../physics/physics_manager.h"
#include "../ui/core/ui_element.h"

namespace engine::core
{
class Updatable;
}

namespace engine::input
{
struct InputEvent;
struct InputSnapshot;
class InputEventReceiver;
class InputSnapshotReceiver;
}

namespace engine::scene
{


class Scene
{
public:
	Scene() : camera(1280, 720) {};
	virtual ~Scene() = default;

	virtual void on_enter() = 0;
	virtual void on_exit() = 0;
	virtual void reset() = 0;

	virtual void on_update(double delta);

	virtual void on_render(SDL_Renderer* renderer);

	//imgui debug
	virtual void on_imgui() {}

	virtual void on_input(const engine::input::InputSnapshot& input,const std::vector<engine::input::InputEvent>& events);

	void destroy_all_scene_objects();

	void pause() { _paused = true; }
	void resume() { _paused = false; }
	[[nodiscard]] bool is_paused()const { return _paused; }

	template <typename T, typename... Args>
	T* create_and_add_object(Args&&... args)
	{
		static_assert(
			std::is_base_of_v<engine::core::GameObject, T> || std::is_base_of_v<engine::ui::UiElement, T>,
			"T must derive from engine::core::GameObject or engine::ui::UiElement.");

		return add_object(
			std::make_unique<T>(std::forward<Args>(args)...)
		);
	}

	template <typename T>
	T* add_object(std::unique_ptr<T> object)
	{
		static_assert(
			std::is_base_of_v<engine::core::SceneObject, T>,
			"T must derive from engine::core::SceneObject.");

		static_assert(
			std::is_base_of_v<engine::core::GameObject, T> || std::is_base_of_v<engine::ui::UiElement, T>,
			"T must derive from engine::core::GameObject or engine::ui::UiElement.");

		if (!object)
			return nullptr;

		T* raw_object = object.get();
		bool added = false;

		if constexpr (std::is_base_of_v<engine::core::GameObject, T>)
			added = add_game_object(std::move(object));
		else if constexpr (std::is_base_of_v<engine::ui::UiElement, T>)
			added = add_ui_root(std::move(object));

		if (!added)
			return nullptr;

		register_scene_object_interfaces(raw_object);

		return raw_object;
	}

private:
	void register_scene_object_interfaces(engine::core::SceneObject* object);

protected:
	bool _paused = false;
	[[nodiscard]] engine::physics::PhysicsManager& physics_manager() noexcept { return _physics_manager; }
	[[nodiscard]] const engine::physics::PhysicsManager& physics_manager() const noexcept { return _physics_manager; }

private:
	void remove_destroyed_objects();
	bool add_game_object(std::unique_ptr<engine::core::GameObject> object);
	bool add_ui_root(std::unique_ptr<engine::ui::UiElement> object);

	struct UpdatableEntry
	{
		engine::core::SceneObject* object = nullptr;
		engine::core::Updatable* updatable = nullptr;
	};

	struct InputSnapshotReceiverEntry
	{
		engine::core::SceneObject* object = nullptr;
		engine::input::InputSnapshotReceiver* receiver = nullptr;
	};

	struct InputEventReceiverEntry
	{
		engine::core::SceneObject* object = nullptr;
		engine::input::InputEventReceiver* receiver = nullptr;
	};


protected:
	engine::camera::Camera camera;

private:
	std::array<std::vector<std::unique_ptr<engine::core::GameObject>>,
		static_cast<size_t>(engine::core::DepthLayer::Count)> _object_layers;
	std::vector<std::unique_ptr<engine::ui::UiElement>> _ui_roots;

	std::vector<UpdatableEntry> _updatables;
	std::vector<InputSnapshotReceiverEntry> _snapshot_receivers;
	std::vector<InputEventReceiverEntry> _event_receivers;
	engine::physics::PhysicsManager _physics_manager;
};
}
