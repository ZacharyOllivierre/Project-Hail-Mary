#include "room_scene.h"

#include "../../engine/input/input_state.h"
#include "../projectile.h"
#include "../../engine/core/render/sdl_render_command_executor.h"
#include "../map/dungeon_room.h"

#include<Windows.h>

#include <memory>
#include <vector>

namespace
{
    // phys debug
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

// Gotten from application.h, shouldnt be re defined but vals are private in application class
static constexpr int kLogicalWidth = 1280;
static constexpr int kLogicalHeight = 720;

// Converts window coordinates to logical coordinates for bullet aiming
static SDL_FPoint to_logical_mouse_position(int window_mouse_x, int window_mouse_y) noexcept
{
    SDL_Window *window = SDL_GetMouseFocus();
    if (!window)
        return SDL_FPoint{static_cast<float>(window_mouse_x), static_cast<float>(window_mouse_y)};

    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(window, &window_width, &window_height);
    if (window_width <= 0 || window_height <= 0)
        return SDL_FPoint{static_cast<float>(window_mouse_x), static_cast<float>(window_mouse_y)};

    const float logical_aspect = static_cast<float>(kLogicalWidth) / static_cast<float>(kLogicalHeight);
    const float window_aspect = static_cast<float>(window_width) / static_cast<float>(window_height);

    float viewport_x = 0.0f;
    float viewport_y = 0.0f;
    float viewport_width = static_cast<float>(window_width);
    float viewport_height = static_cast<float>(window_height);

    if (window_aspect > logical_aspect)
    {
        viewport_width = viewport_height * logical_aspect;
        viewport_x = (static_cast<float>(window_width) - viewport_width) * 0.5f;
    }
    else
    {
        viewport_height = viewport_width / logical_aspect;
        viewport_y = (static_cast<float>(window_height) - viewport_height) * 0.5f;
    }

    return SDL_FPoint{
        (static_cast<float>(window_mouse_x) - viewport_x) * static_cast<float>(kLogicalWidth) / viewport_width,
        (static_cast<float>(window_mouse_y) - viewport_y) * static_cast<float>(kLogicalHeight) / viewport_height};
}

void RoomScene::on_enter()
{
    _paused = false;

    build_room();
    spawn_player();
}

void RoomScene::on_update(double delta)
{
    //Sleep(100);

    this->Scene::on_update(delta);

    if (_player && !_player->is_destroyed() && !_player->is_dead())
    {
        camera.follow(_player->center().x, _player->center().y, 1.0f);
    }
}

void RoomScene::on_render(SDL_Renderer *renderer)
{
    this->Scene::on_render(renderer);

    // phys debug
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

    // phys debug
    for (const InputEvent& event : events)
    {
        if (event.action == InputAction::Tab
            && event.type == InputEventType::Pressed)
        {
            physics_manager().set_debug_enabled(!physics_manager().debug_enabled());
        }
    }

    if (!_player || _player->is_destroyed() || _player->is_dead())
        return;

    if (input.state.is_just_pressed(InputAction::Attack))
    {
        int mouse_x = 0;
        int mouse_y = 0;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        const SDL_FPoint logical_mouse = to_logical_mouse_position(mouse_x, mouse_y);

        const SDL_FPoint mouse_world = camera.screen_to_world(
            logical_mouse.x,
            logical_mouse.y);

        Vector2 aim_direction = Vector2(mouse_world.x, mouse_world.y) - _player->center();
        if (aim_direction.is_zero())
            aim_direction = Vector2(1.0f, 0.0f);

        const Vector2 shot_direction = aim_direction.normalized();

        std::unique_ptr<Projectile> projectile = _player->create_projectile(shot_direction);

        if (!projectile)
            return;

        Projectile *added_projectile = add_object(std::move(projectile));
        if (!added_projectile)
            return;

        physics_manager().register_body(
            added_projectile,
            added_projectile,
            added_projectile);
    }
}

void RoomScene::spawn_effect(const EffectSpawnRequest &request)
{
    std::unique_ptr<Effect> effect = EffectManager::instance()->create_effect(request);
    if (!effect)
        return;

    add_object(std::move(effect));
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
        _player->set_move_speed(200.0f);
        physics_manager().register_body(_player, _player, _player);
    }
}
