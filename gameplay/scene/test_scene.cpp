#include "test_scene.h"

#include "../../engine/core/geometry/rect.h"
#include "../../engine/core/render/sdl_convert.h"

void TestScene::on_enter()
{
	_paused = false;
	_t_obj = std::make_shared<TestObj>();
	Scene::add_object(_t_obj);
	//Scene::add_object(std::move(_t_obj));
}

void TestScene::on_update(double delta)
{
	Scene::on_update(delta);

	if (_rect.contains(_t_obj.get()->world_rect()))
		_contain = true;
	else
		_contain = false;
}

void TestScene::on_render(SDL_Renderer* renderer)
{
	Scene::on_render(renderer);


	const SDL_Rect square_rect = to_sdl_rect(_rect);

	Uint8 draw_r = 0;Uint8 draw_g = 0;Uint8 draw_b = 0;Uint8 draw_a = 0;
	SDL_GetRenderDrawColor(renderer, &draw_r, &draw_g, &draw_b, &draw_a);


	if (_contain)
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	else
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_RenderDrawRect(renderer, &square_rect);

	SDL_SetRenderDrawColor(renderer, draw_r, draw_g, draw_b, draw_a);

}

void TestScene::on_input(const InputSnapshot& input,const std::vector<InputEvent>& events)
{
	Scene::on_input(input, events);
}


void TestScene::on_exit()
{
	clear_objects();
	_paused = false;
}

void TestScene::reset()
{
	clear_objects();
	_paused = false;
}
