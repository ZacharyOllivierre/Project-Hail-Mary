#include "test_scene.h"

#include "../../engine/core/geometry/rect.h"
#include "../../engine/core/render/sdl_convert.h"

void TestScene::spawn_test_objects()
{
	_test_object = create_and_add_object<TestObject>();
	_test_map = create_and_add_object<TestMap>();
}

void TestScene::destroy_tracked_objects()
{
	if (_test_object && !_test_object->is_destroyed())
		_test_object->destroy();

	if (_test_map && !_test_map->is_destroyed())
		_test_map->destroy();

	_test_object = nullptr;
	_test_map = nullptr;
}

void TestScene::on_enter()
{
	_paused = false;
	_contain = false;
	spawn_test_objects();
}

void TestScene::on_update(double delta)
{
	Scene::on_update(delta);

	if (!_test_object)
	{
		_contain = false;
		return;
	}

	_contain = _rect.contains(_test_object->world_rect());
	camera.follow(_test_object->center().x, _test_object->center().y, 1);
}

void TestScene::on_render(SDL_Renderer *renderer)
{
	Scene::on_render(renderer);

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

void TestScene::on_input(const InputSnapshot &input, const std::vector<InputEvent> &events)
{
	Scene::on_input(input, events);

	if (_test_object && _test_object->is_destroyed())
		_test_object = nullptr;
}

void TestScene::on_exit()
{
	destroy_tracked_objects();
	_paused = false;
	_contain = false;
}

void TestScene::reset()
{
	destroy_tracked_objects();
	_paused = false;
	_contain = false;
	spawn_test_objects();
}
