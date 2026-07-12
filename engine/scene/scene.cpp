#include "scene.h"

#include "../core/collision_manager.h"
#include "../core/interface/updatable.h"
#include "../core/render/sdl_render_command_executor.h"

#include "../input/contracts/input_event_receiver.h"
#include "../input/contracts/input_snapshot_receiver.h"

#include <algorithm>
#include <functional>
#include <tuple>

namespace
{
template <typename Entry>
void erase_destroyed_entries(std::vector<Entry>& entries)
{
	std::erase_if(entries, [](const Entry& entry)
	{return !entry.object || entry.object->is_destroyed();});
}

[[nodiscard]] bool scene_object_input_event_less(
	const SceneObject* lhs,const SceneObject* rhs
) noexcept
{
	if (lhs == rhs)
		return false;

	const UiElement* lhs_ui = dynamic_cast<const UiElement*>(lhs);
	const UiElement* rhs_ui = dynamic_cast<const UiElement*>(rhs);

	if (lhs_ui && rhs_ui)
		return lhs_ui->order() > rhs_ui->order();

	const GameObject* lhs_game = dynamic_cast<const GameObject*>(lhs);
	const GameObject* rhs_game = dynamic_cast<const GameObject*>(rhs);

	if (lhs_game && rhs_game)
	{
		return std::make_tuple(
			lhs_game->depth_layer(),
			lhs_game->order_in_layer()
		) > std::make_tuple(
			rhs_game->depth_layer(),
			rhs_game->order_in_layer()
		);
	}

	if (lhs_ui && rhs_game)
		return true;

	if (lhs_game && rhs_ui)
		return false;

	return std::less<const SceneObject*>{}(lhs, rhs);
}

template <typename Entry>
void insert_input_event_entry_sorted(std::vector<Entry>& entries, Entry entry)
{
	auto iter = std::upper_bound(entries.begin(), entries.end(), entry,
		[](const Entry& lhs, const Entry& rhs)
		{	return scene_object_input_event_less(lhs.object, rhs.object);}
	);

	entries.insert(iter, entry);
}


}

void Scene::on_input(const InputSnapshot& input,const std::vector<InputEvent>& events)
{
	for (const InputSnapshotReceiverEntry& entry : _snapshot_receivers)
	{
		SceneObject* object = entry.object;

		if (!object || object->is_destroyed() || !object->is_active())
			continue;

		if (_paused && !object->receive_input_when_paused())
			continue;

		entry.receiver->on_input_snapshot(input);
	}

	for (const InputEvent& input_event : events)
	{
		for (const InputEventReceiverEntry& entry : _event_receivers)
		{
			SceneObject* object = entry.object;

			if (!object || object->is_destroyed() || !object->is_active())
				continue;

			if (_paused && !object->receive_input_when_paused())
				continue;

			if (entry.receiver->on_input_event(input_event))
				break;
		}
	}
}

void Scene::on_update(double delta)
{
	for (const UpdatableEntry& entry : _updatables)
	{
		SceneObject* object = entry.object;

		if (!object || object->is_destroyed() || !object->is_active())
			continue;

		if (_paused && !object->update_when_paused())
			continue;

		entry.updatable->update(delta);
	}

	if (!_paused)
	{
		_physics_manager.step(delta);
		CollisionManager::instance()->update();
	}

	remove_destroyed_objects();
}

void Scene::on_render(SDL_Renderer* renderer)
{
	if (!renderer)
		return;

	std::vector<RenderCommand> render_commands;
	std::vector<UiRenderCommand> ui_render_commands;
	render_commands.reserve(256);
	ui_render_commands.reserve(256);

	for (const auto& layer : _object_layers)
	{
		render_commands.clear();

		for (const std::unique_ptr<GameObject>& obj : layer)
		{
			if (!obj || obj->is_destroyed() || !obj->is_visible())
				continue;

			obj->submit_render_commands(render_commands);
		}

		// Translate world rec to screenRec
		for (RenderCommand& command : render_commands)
		{
			command.command_rect = camera.world_to_screen(command.command_rect);
		}

		execute_render_commands(renderer, render_commands);
	}


	ui_render_commands.clear();
	for (const auto& ui_root : _ui_roots)
	{
		if (!ui_root || ui_root->is_destroyed() || !ui_root->is_visible())
			continue;

		ui_root->submit_ui_render_commands(ui_render_commands);
	}

	execute_render_commands(renderer, ui_render_commands);
}

void Scene::destroy_all_scene_objects()
{
	for (auto& layer : _object_layers)
	{
		for (std::unique_ptr<GameObject>& object : layer)
		{
			if (object)
				object->destroy();
		}
	}

	for (std::unique_ptr<UiElement>& ui_root : _ui_roots)
	{
		if (ui_root)
			ui_root->destroy();
	}

	_physics_manager.clear_collision_world();
	_physics_manager.clear_bodies();
	CollisionManager::instance()->clear();

	remove_destroyed_objects();
}

void Scene::register_scene_object_interfaces(SceneObject* object)
{
	if (!object)
		return;

	if (Updatable* updatable = dynamic_cast<Updatable*>(object))
		_updatables.push_back(UpdatableEntry{ object,updatable });

	if (InputSnapshotReceiver* receiver =dynamic_cast<InputSnapshotReceiver*>(object))
		_snapshot_receivers.push_back(InputSnapshotReceiverEntry{ object,receiver });

	if (InputEventReceiver* receiver =dynamic_cast<InputEventReceiver*>(object))
		insert_input_event_entry_sorted(_event_receivers,InputEventReceiverEntry{object,receiver});
}

void Scene::remove_destroyed_objects()
{
	erase_destroyed_entries(_updatables);
	erase_destroyed_entries(_snapshot_receivers);
	erase_destroyed_entries(_event_receivers);

	for (auto& layer : _object_layers)
	{
		std::erase_if(layer, [](const std::unique_ptr<GameObject>& object)
			{
				return !object || object->is_destroyed();
			});
	}

	std::erase_if(_ui_roots, [](const std::unique_ptr<UiElement>& object)
		{
			return !object || object->is_destroyed();
		});
}

bool Scene::add_game_object(std::unique_ptr<GameObject> object)
{
	if (!object)
		return false;

	const size_t layer_index = static_cast<size_t>(object->depth_layer());

	if (layer_index >= _object_layers.size())
		return false;

	std::vector<std::unique_ptr<GameObject>>& layer = _object_layers[layer_index];

	auto iter = std::upper_bound(
		layer.begin(),
		layer.end(),
		object->order_in_layer(),
		[](int order, const std::unique_ptr<GameObject>& existing)
		{
			return order < existing->order_in_layer();
		}
	);

	layer.insert(iter, std::move(object));
	return true;
}

bool Scene::add_ui_root(std::unique_ptr<UiElement> object)
{
	if (!object)
		return false;

	auto iter = std::upper_bound(
		_ui_roots.begin(),
		_ui_roots.end(),
		object->order(),
		[](int order, const std::unique_ptr<UiElement>& existing)
		{
			return order < existing->order();
		}
	);

	_ui_roots.insert(iter, std::move(object));
	return true;
}
