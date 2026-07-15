#include "game_scene.h"

#include "../../engine/animation/effect_manager.h"
#include "../../engine/core/geometry/rect.h"
#include "../../engine/core/render/render_command.h"
#include "../../engine/core/render/sdl_convert.h"
#include "../../engine/input/input_state.h"
#include "../../engine/resources/resource_manager.h"

#include <memory>
#include <utility>

namespace
{
	class GameplayMap final : public engine::core::GameObject
	{
	public:
		GameplayMap()
			: engine::core::GameObject(engine::core::DepthLayer::Item)
		{
			_texture = engine::resources::ResourceManager::instance()->find_texture("room_tiles");
			set_position({10.0f, 10.0f});
			set_size({3344.0f, 1882.0f});
		}

		void submit_render_commands(std::vector<engine::core::RenderCommand> &out_commands) const override
		{
			engine::core::RenderCommand command;
			command.command_rect = world_rect();
			command.texture = _texture;
			out_commands.push_back(std::move(command));
		}

	private:
		SDL_Texture *_texture = nullptr;
	};
}

void GameScene::on_enter()
{
	_paused = false;
	_contain = false;
	spawn_map();
	spawn_player();
}

void GameScene::on_update(double delta)
{
	engine::scene::Scene::on_update(delta);

	if (!_player || _player->is_destroyed() || _player->is_dead())
	{
		_contain = false;
		return;
	}

	_contain = _rect.contains(_player->world_rect());
	camera.follow(_player->center().x, _player->center().y, 1);
}

void GameScene::on_render(SDL_Renderer *renderer)
{
	engine::scene::Scene::on_render(renderer);

	if (!renderer)
		return;

	const SDL_Rect square_rect = to_sdl_rect(camera.world_to_screen(_rect));

	Uint8 draw_r = 0;
	Uint8 draw_g = 0;
	Uint8 draw_b = 0;
	Uint8 draw_a = 0;
	SDL_GetRenderDrawColor(renderer, &draw_r, &draw_g, &draw_b, &draw_a);

	if (_contain)
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	else
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_RenderDrawRect(renderer, &square_rect);

	SDL_SetRenderDrawColor(renderer, draw_r, draw_g, draw_b, draw_a);
}

void GameScene::on_input(const engine::input::InputSnapshot &input, const std::vector<engine::input::InputEvent> &events)
{
	engine::scene::Scene::on_input(input, events);
	consume_player_effect_requests();

	if (_player && _player->is_destroyed())
		_player = nullptr;

	if ((!_player || _player->is_destroyed() || _player->is_dead()) &&
		input.state.is_just_pressed(engine::input::InputAction::Attack))
	{
		spawn_player();
	}
}

void GameScene::on_exit()
{
	destroy_tracked_objects();
	_paused = false;
	_contain = false;
}

void GameScene::reset()
{
	destroy_tracked_objects();
	_paused = false;
	_contain = false;
	spawn_map();
	spawn_player();
}

void GameScene::spawn_player()
{
	if (_player && !_player->is_destroyed() && !_player->is_dead())
		return;

	_player = create_and_add_object<PlayerCharacter>(
		"elves",
		engine::core::Vector2(200.0f, 200.0f),
		engine::core::Vector2(100.0f, 100.0f),
		"fire.impact_radial");

	if (_player)
	{
		physics_manager().register_body(_player, _player, _player);
	}
}

void GameScene::spawn_map()
{
	if (_map)
		return;

	_map = add_object(std::make_unique<GameplayMap>());
}

void GameScene::destroy_tracked_objects()
{
	destroy_all_scene_objects();
	_player = nullptr;
	_map = nullptr;
}

void GameScene::consume_player_effect_requests()
{
	if (!_player || _player->is_destroyed() || _player->is_dead())
		return;

	std::vector<engine::animation::EffectSpawnRequest> effect_requests =
		_player->drain_effect_spawn_requests();

	for (const engine::animation::EffectSpawnRequest &request : effect_requests)
	{
		std::unique_ptr<engine::animation::Effect> effect =
			engine::animation::EffectManager::instance()->create_effect(request);
		if (!effect)
			continue;

		add_object(std::move(effect));
	}
}
