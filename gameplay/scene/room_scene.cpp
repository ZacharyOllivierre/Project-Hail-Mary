#include "room_scene.h"

#include "../../engine/input/input_state.h"
#include "../map/dungeon_room.h"

#include <memory>

void RoomScene::on_enter()
{
    _paused = false;

    build_room();
    spawn_player();
}

void RoomScene::on_update(double delta)
{
    this->Scene::on_update(delta);

    if (_player && !_player->is_destroyed() && !_player->is_dead())
    {
        camera.follow(_player->center().x, _player->center().y, 1.0f);
    }
}

void RoomScene::on_render(SDL_Renderer *renderer)
{
    this->Scene::on_render(renderer);
}

void RoomScene::on_input(const InputSnapshot &input, const std::vector<InputEvent> &events)
{
    this->Scene::on_input(input, events);
}

void RoomScene::on_exit()
{
    this->destroy_all_scene_objects();
    _collision_world.set_room(nullptr);
    _player = nullptr;
    _paused = false;
    _room = nullptr;
}

void RoomScene::reset()
{
    this->destroy_all_scene_objects();
    _collision_world.set_room(nullptr);
    _player = nullptr;
    _paused = false;
    _room = nullptr;
    build_room();
    spawn_player();
}

void RoomScene::build_room()
{
    if (_room)
        return;

    _room = add_object(std::make_unique<DungeonRoom>());
    if (_room)
    {
        _collision_world.set_room(_room);
        physics_manager().set_collision_world(&_collision_world);
    }
}

void RoomScene::spawn_player()
{
    if (_player && !_player->is_destroyed() && !_player->is_dead())
        return;

    _player = create_and_add_object<Character>(
        "elves",
        Vector2(520.0f, 580.0f),
        Vector2(64.0f, 64.0f),
        "fire.impact_radial");

    if (_player)
    {
        _player->set_move_speed(240.0f);
        physics_manager().register_body(_player, _player, _player);
    }
}

