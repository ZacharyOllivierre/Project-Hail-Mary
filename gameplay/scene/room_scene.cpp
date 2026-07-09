#include "room_scene.h"

#include "../../engine/input/input_state.h"
#include "../../engine/core/render/sdl_render_command_executor.h"
#include "../map/dungeon_room.h"

#include <memory>
#include <vector>

namespace
{
    [[nodiscard]] SDL_Color debug_color_for(PhysicsManager::DebugRectType type) noexcept
    {
        switch (type)
        {
        case PhysicsManager::DebugRectType::Collider:
            return SDL_Color{ 0, 255, 80, 255 };
        case PhysicsManager::DebugRectType::SubstepCollider:
            return SDL_Color{ 140, 255, 180, 255 };
        case PhysicsManager::DebugRectType::HorizontalCandidate:
            return SDL_Color{ 255, 220, 0, 255 };
        case PhysicsManager::DebugRectType::VerticalCandidate:
            return SDL_Color{ 0, 220, 255, 255 };
        case PhysicsManager::DebugRectType::BlockingTile:
            return SDL_Color{ 255, 40, 40, 255 };
        default:
            return SDL_Color{ 255, 255, 255, 255 };
        }
    }
}

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

    if (!renderer || !physics_manager().debug_enabled())
        return;

    const std::vector<PhysicsManager::DebugRect>& debug_snapshot =
        physics_manager().debug_snapshot();
    if (debug_snapshot.empty())
        return;

    std::vector<UiRenderCommand> debug_commands;
    debug_commands.reserve(debug_snapshot.size());

    for (const PhysicsManager::DebugRect& debug_rect : debug_snapshot)
    {
        debug_commands.push_back(make_ui_draw_rect_command(
            camera.world_to_screen(debug_rect.rect),
            debug_color_for(debug_rect.type)
        ));
    }

    execute_render_commands(renderer, debug_commands);
}

void RoomScene::on_input(const InputSnapshot &input, const std::vector<InputEvent> &events)
{
    this->Scene::on_input(input, events);

    for (const InputEvent& event : events)
    {
        if (event.action == InputAction::Attack
            && event.type == InputEventType::Pressed)
        {
            physics_manager().set_debug_enabled(!physics_manager().debug_enabled());
        }
    }
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
        Vector2(540.0f, 540.0f),
        Vector2(64.0f, 64.0f),
        "fire.impact_radial");

    if (_player)
    {
        _player->set_move_speed(100000.0f);
        physics_manager().register_body(_player, _player, _player);
    }
}

