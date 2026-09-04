#include "prepare_scene.h"

#include "../../engine/tools/logger.h"

void PrepareScene::on_enter()
{
	_paused = false;
	ENGINE_LOG_INFO("scene", "Entered PrepareScene");
}

void PrepareScene::on_update(double delta)
{
	engine::scene::Scene::on_update(delta);
}

void PrepareScene::on_render(SDL_Renderer* renderer)
{
	engine::scene::Scene::on_render(renderer);
}

void PrepareScene::on_input(
	const engine::input::InputSnapshot& input,
	const std::vector<engine::input::InputEvent>& events)
{
	engine::scene::Scene::on_input(input, events);
}

void PrepareScene::on_exit()
{
	ENGINE_LOG_INFO("scene", "Leaving PrepareScene");
	destroy_all_scene_objects();
	_paused = false;
}

void PrepareScene::reset()
{
	destroy_all_scene_objects();
	_paused = false;
}
