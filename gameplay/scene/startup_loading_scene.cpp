#include "startup_loading_scene.h"

#include "../../engine/scene/scene_manager.h"
#include "../../engine/input/input_state.h"
#include "../../engine/resources/resource_bootstrapper.h"


#include "game_scene.h"

#include <iostream>

void StartUpLoadingScene::on_enter()
{

}

void StartUpLoadingScene::on_update(double delta)
{
	Scene::on_update(delta);
}

void StartUpLoadingScene::on_render(SDL_Renderer* renderer)
{
	Scene::on_render(renderer);
}

// Handles menu input, including keyboard input for starting the game.
void StartUpLoadingScene::on_input(
	const InputSnapshot& input,
	const std::vector<InputEvent>& events
)
{
	(void)events;

	Scene::on_input(input, events);
}

// Runs when menu scene is exited.
void StartUpLoadingScene::on_exit()
{
	std::cout << "Leaving MenuScene" << std::endl;

}

// Resets menu scene back to its starting state.
void StartUpLoadingScene::reset()
{

}

