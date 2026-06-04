#include "test_scene.h"

void TestScene::on_enter()
{
	_paused = false;
}

void TestScene::on_update()
{

}

void TestScene::on_render(SDL_Renderer* renderer)
{

}

void TestScene::on_input()
{

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
