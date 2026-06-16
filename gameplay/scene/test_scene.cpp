#include "test_scene.h"

#include "../../engine/core/geometry/rect.h"
#include "../../engine/core/render/sdl_convert.h"
#include "../../engine/input/input_state.h"

#include <iostream>

void TestScene::spawn_test_object()
{
	if (_test_object)
		return;

	_test_object = add_object(_test_object_factory.create());
}

void TestScene::spawn_test_ui()
{
	if (_test_ui)
		return;

	_test_ui = create_and_add_object<TestUi>();
}

void TestScene::destroy_tracked_objects()
{
	if (_test_object && !_test_object->is_destroyed())
		_test_object->destroy();

	if (_test_map && !_test_map->is_destroyed())
		_test_map->destroy();

	if (_test_ui && !_test_ui->is_destroyed())
		_test_ui->destroy();

	_test_object = nullptr;
	_test_map = nullptr;
	_test_ui = nullptr;
}

void TestScene::on_enter()
{
	_test_map = add_object(std::make_unique<TestMap>());
	_paused = false;
	_contain = false;
	spawn_test_object();
	spawn_test_ui();
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

	// Added for button
	// Create button only one, on_render runs every frame, so check to stop duplication
    if (!_test_button)
    {

		// Create a button at x = 100, y = 100, width = 200, height = 60
        SDL_Rect button_rect{ 100, 100, 200, 60 };

        _test_button = std::make_unique<Button>(renderer, button_rect);

        _test_button->set_on_click([]()
        {
            std::cout << "Test button clicked!" << std::endl;
        });
    }

	// Draws button every frame after its been created
    _test_button->render();
	// Added for button end

}

void TestScene::on_input(const InputSnapshot &input, const std::vector<InputEvent> &events)
{
	Scene::on_input(input, events);

	if (_test_object && _test_object->is_destroyed())
		_test_object = nullptr;

	if (!_test_object && input.state.is_just_pressed(InputAction::Attack))
		spawn_test_object();
}

void TestScene::on_exit()
{
	destroy_tracked_objects();
	_paused = false;
	_contain = false;

	// Added for button
	// Deletes button when leaving scene, prevents button from being in memory when scene ends
	_test_button.reset();
	// Added for button end
}

void TestScene::reset()
{
	destroy_tracked_objects();
	_paused = false;
	_contain = false;
	spawn_test_object();
	spawn_test_ui();

}

// Added for button
// Lets application send raw SDL events to this scene, like mouse movement
void TestScene::handle_sdl_event(const SDL_Event& event)
{
	// Only forward event if button already exists, button decides if event matters
	// Like checking if a mouse clicked a rectangle

    if (_test_button)
    {
        _test_button->handle_event(event);
    }
}
// Added for button end