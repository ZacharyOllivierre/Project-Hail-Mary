#pragma once

#include <SDL.h>
#include <algorithm>
#include <array>
#include <memory>
#include <vector>

#include "../core/depth_layer.h"
#include "../core/game_object.h"
#include "../core/interface/updatable.h"
#include "../core/render/sdl_render_command_executor.h"
#include "../input/contracts/input_event_receiver.h"
#include "../input/contracts/input_snapshot_receiver.h"

class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual void on_enter() = 0;
	virtual void on_exit() = 0;

	virtual void on_update(double delta)
	{
		for (auto& layer : _object_layers)
		{
			const std::vector<std::shared_ptr<GameObject>> objects = layer;
			for (const std::shared_ptr<GameObject>& obj : objects)
			{
				if (!obj || obj->is_destroyed())
					continue;

				if (!obj->is_active())
					continue;

				Updatable* updatable = dynamic_cast<Updatable*>(obj.get());
				if (!updatable)
					continue;

				updatable->update(obj->scaled_delta(delta));
			}
		}

		remove_destroyed_objects();
	}

	virtual void on_render(SDL_Renderer* renderer)
	{
		std::vector<RenderCommand> render_commands;
		render_commands.reserve(512);

		for (auto& layer : _object_layers)
		{
			const std::vector<std::shared_ptr<GameObject>> objects = layer;
			for (const std::shared_ptr<GameObject>& obj : objects)
			{
				if (!obj || obj->is_destroyed())
					continue;

				if (!obj->is_visible())
					continue;

				obj->submit_render_commands(render_commands);
			}
			execute_render_commands(renderer, render_commands);
		}

	}

	virtual void on_input(
		const InputSnapshot& input,
		const std::vector<InputEvent>& events
	)
	{
		for (auto& layer : _object_layers)
		{
			const std::vector<std::shared_ptr<GameObject>> objects = layer;
			for (const std::shared_ptr<GameObject>& obj : objects)
			{
				if (!obj || obj->is_destroyed())
					continue;

				if (!obj->is_active())
					continue;

				if (_paused)
					continue;

				InputSnapshotReceiver* snapshot_receiver =
					dynamic_cast<InputSnapshotReceiver*>(obj.get());
				if (snapshot_receiver)
					snapshot_receiver->on_input_snapshot(input);
			}
		}

		for (const InputEvent& input_event : events)
		{
			bool consumed = false;

			for (auto& layer : _object_layers)
			{
				const std::vector<std::shared_ptr<GameObject>> objects = layer;
				for (const std::shared_ptr<GameObject>& obj : objects)
				{
					if (!obj || obj->is_destroyed())
						continue;

					if (!obj->is_active())
						continue;

					InputEventReceiver* event_receiver =
						dynamic_cast<InputEventReceiver*>(obj.get());
					if (!event_receiver)
						continue;

					if (event_receiver->on_input_event(input_event))
					{
						consumed = true;
						break;
					}
				}

				if (consumed)
					break;
			}
		}
	}

	virtual void reset() = 0;

	void add_object(std::shared_ptr<GameObject> obj)
	{
		if (!obj)
			return;

		auto& layer = _object_layers[to_index(obj->depth_layer())];

		auto iter = std::lower_bound(layer.begin(), layer.end(), obj,
			[](const auto& a, const auto& b)
			{
				return a->order_in_layer() < b->order_in_layer();
			}
		);

		layer.insert(iter, obj);
	}

	void clear_objects()
	{
		for (auto& layer : _object_layers)
			layer.clear();
	}

	void pause() { _paused = true; }
	void resume() { _paused = false; }
	[[nodiscard]] bool is_paused() const { return _paused; }

protected:
	static constexpr size_t to_index(DepthLayer layer)
	{
		return static_cast<size_t>(layer);
	}

	void reset_objects()
	{
		for (auto& layer : _object_layers)
		{
			const std::vector<std::shared_ptr<GameObject>> objects = layer;
			for (const std::shared_ptr<GameObject>& obj : objects)
			{
				if (obj)
					obj->reset();
			}
		}
	}

	void remove_destroyed_objects()
	{
		for (auto& layer : _object_layers)
		{
			layer.erase(
				std::remove_if(layer.begin(), layer.end(),
					[](const auto& obj)
					{
						return !obj || obj->is_destroyed();
					}),
				layer.end()
			);
		}
	}

protected:
	bool _paused = false;

	std::array<
		std::vector<std::shared_ptr<GameObject>>,
		static_cast<size_t>(DepthLayer::Count)
	> _object_layers;
};
