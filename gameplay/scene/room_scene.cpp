#include "room_scene.h"

#include "../../engine/physics/collision_manager.h"
#include "../../engine/input/input_state.h"
#include "../combat/bullet.h"
#include "../combat/projectile.h"
#include "../map/dungeon_room.h"
#include "../../thirdparty/imgui/imgui.h"

#include <memory>
#include <vector>

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
    spawn_enemies();
}

void RoomScene::on_update(double delta)
{
    this->engine::scene::Scene::on_update(delta);

    spawn_scheduled_projectiles(delta);

    if (_player && !_player->is_destroyed() && !_player->is_dead())
    {
        camera.follow(_player->center().x, _player->center().y, 1.0f);
    }
}

void RoomScene::on_render(SDL_Renderer *renderer)
{
    this->engine::scene::Scene::on_render(renderer);
}

void RoomScene::on_input(const engine::input::InputSnapshot &input, const std::vector<engine::input::InputEvent> &events)
{
    this->engine::scene::Scene::on_input(input, events);

    if (!_player || _player->is_destroyed() || _player->is_dead())
        return;

    if (input.state.is_just_pressed(engine::input::InputAction::Attack))
    {
        const engine::core::Vector2 shot_direction = get_shot_direction();

        // Get schedule of projectiles from wand attack and add to buffer
        vector<ShotDescriptor> shots = _player->create_projectile(shot_direction);
        _scheduled_projectiles.insert(_scheduled_projectiles.end(), shots.begin(), shots.end());
    }
}

void RoomScene::spawn_effect(const engine::animation::EffectSpawnRequest &request)
{
    std::unique_ptr<engine::animation::Effect> effect = engine::animation::EffectManager::instance()->create_effect(request);
    if (!effect)
        return;

    add_object(std::move(effect));
}

engine::core::Vector2 RoomScene::closest_enemy_to_point(engine::core::Vector2 &point)
{
    if (_enemies.empty())
    {
        return engine::core::Vector2(0, 0);
    }

    engine::core::Vector2 closest = _enemies[0]->center();
    float closest_dist_sq = (closest - point).length_squared();

    for (int i = 1; i < _enemies.size(); i++)
    {
        float dist_sq = (_enemies[i]->center() - point).length_squared();

        if (dist_sq < closest_dist_sq)
        {
            closest = _enemies[i]->center();
            closest_dist_sq = dist_sq;
        }
    }

    return closest;
}

// imgui debug
void RoomScene::on_imgui()
{
    if (_player && !_player->is_destroyed())
    {
        ImGui::Separator();
        _player->wand().debug_data().render_debugger();
    }
}

void RoomScene::on_exit()
{
    this->destroy_all_scene_objects();
    _scheduled_projectiles.clear();
    _collision_world.set_room(nullptr);
    _player = nullptr;
    _paused = false;
    _room = nullptr;
}

void RoomScene::reset()
{
    this->destroy_all_scene_objects();
    _scheduled_projectiles.clear();
    _collision_world.set_room(nullptr);
    _player = nullptr;
    _paused = false;
    _room = nullptr;
    build_room();
    spawn_player();
}

// Iterate through all scheduled shots and copy over / spawn ready ones
void RoomScene::spawn_scheduled_projectiles(double delta)
{
    // Update projectile timer and spawn any shots which timer is at or below 0
    for (auto it = _scheduled_projectiles.begin(); it != _scheduled_projectiles.end();)
    {
        it->spawn_delay_sec -= delta;

        if (it->spawn_delay_sec > 0.0f)
        {
            ++it;
            continue;
        }

        // Update bullut spawn position with offset relative to player
        Bullet_Attributes &bullet_attributes = it->bullet_attributes;
        bullet_attributes.start_position = _player->center() + it->spawn_offset;
        std::unique_ptr<Projectile> projectile = std::make_unique<Bullet>(bullet_attributes);

        Projectile *added_projectile = add_object(std::move(projectile));
        it = _scheduled_projectiles.erase(it);

        if (!added_projectile)
            continue;

        physics_manager().register_body(
            added_projectile,
            added_projectile,
            added_projectile);

        engine::physics::CollisionBox *collision_box = engine::physics::CollisionManager::instance()->create_box(
            added_projectile,
            engine::physics::CollisionLayer::PlayerProjectile,
            engine::physics::CollisionTarget::Enemy,
            [added_projectile](const engine::physics::CollisionInfo &)
            {
                added_projectile->destroy();
            });

        added_projectile->set_collision_box(collision_box);
    }
}

engine::core::Vector2 RoomScene::get_shot_direction()
{
    // Get direction player is shooting
    int mouse_x = 0;
    int mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    const SDL_FPoint logical_mouse = to_logical_mouse_position(mouse_x, mouse_y);

    const SDL_FPoint mouse_world = camera.screen_to_world(
        logical_mouse.x,
        logical_mouse.y);

    engine::core::Vector2 aim_direction = engine::core::Vector2(mouse_world.x, mouse_world.y) - _player->center();
    if (aim_direction.is_zero())
        aim_direction = engine::core::Vector2(1.0f, 0.0f);

    return aim_direction.normalized();
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

    _player = create_and_add_object<PlayerCharacter>(
        "elves",
        engine::core::Vector2(540.0f, 540.0f),
        engine::core::Vector2(64.0f, 64.0f),
        "fire.impact_radial");

    if (_player)
    {
        _player->set_move_speed(200.0f);
        physics_manager().register_body(_player, _player, _player);
    }
}

void RoomScene::spawn_enemies()
{
    int num = 2;
    int distance = 200;

    for (int i = 0; i < num; i++)
    {
        Enemy *enemy = create_and_add_object<Enemy>(
            "elves",
            engine::core::Vector2(540.0f + distance * i, 540.0f + distance * i),
            engine::core::Vector2(64.0f, 64.0f));

        if (enemy)
        {
            enemy->set_move_speed(0);
            _enemies.push_back(enemy);
            physics_manager().register_body(enemy, enemy, enemy);
        }
    }
}
