#include "game_scene.h"

#include "../../engine/core/geometry/rect.h"
#include "../../engine/core/render/sdl_convert.h"
#include "../../engine/input/input_state.h"


void GameScene::on_enter()
{

}

void GameScene::on_update(double delta)
{
	Scene::on_update(delta);
}

void GameScene::on_render(SDL_Renderer* renderer)
{
	if (!renderer)
		return;

	Scene::on_render(renderer);



}

void GameScene::on_input(const InputSnapshot& input, const std::vector<InputEvent>& events)
{
	Scene::on_input(input, events);
}

void GameScene::on_exit()
{

}

void GameScene::reset()
{

}
