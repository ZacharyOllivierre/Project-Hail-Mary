#include "room_scene.h"

#include "../../engine/physics/collision_manager.h"
#include "../../engine/input/input_state.h"
#include "../combat/bullet.h"
#include "../combat/attack_info.h"
#include "../combat/projectile.h"
#include "../map/dungeon_room.h"
#include "../../thirdparty/imgui/imgui.h"

#include <memory>
#include <vector>

void RoomScene::on_enter()
{
    _paused = false;

    build_room();
    spawn_player();
    generate_enemies(EnemyType::Skeleton, 3);
    generate_enemies(EnemyType::Slime, 3);
    generate_enemies(EnemyType::GoblinWitch, 2);
    generate_enemies(EnemyType::Wizard, 1);
    generate_enemies(EnemyType::SkeletonElite, 2);
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

    if (input.state.is_just_pressed(engine::input::InputAction::Reset))
    {
        reset();
        return;
    }

    if (!_player || _player->is_destroyed() || _player->is_dead())
        return;

    if (input.state.is_just_pressed(engine::input::InputAction::Attack))
    {
        engine::core::Vector2 shot_direction(1.0f, 0.0f);
        if (input.has_pointer_position)
        {
            shot_direction = get_shot_direction(input.pointer_x, input.pointer_y);
        }

        // Get schedule of projectiles from wand attack and add to buffer
        vector<ShotDescriptor> shots = _player->create_projectile(shot_direction);
        _scheduled_projectiles.insert(_scheduled_projectiles.end(), shots.begin(), shots.end());
    }
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
    // if (_player && !_player->is_destroyed())
    // {
    //     ImGui::Separator();
    //     _player->wand().debug_data().render_debugger();
    // }
}

void RoomScene::on_exit()
{
    this->destroy_all_scene_objects();
    _enemies.clear();
    _scheduled_projectiles.clear();
    _collision_world.set_room(nullptr);
    _player = nullptr;
    _paused = false;
    _room = nullptr;
}

void RoomScene::reset()
{
    this->destroy_all_scene_objects();
    _enemies.clear();
    _scheduled_projectiles.clear();
    _collision_world.set_room(nullptr);
    _player = nullptr;
    _paused = false;
    _room = nullptr;
    build_room();
    spawn_player();
    generate_enemies(EnemyType::Skeleton, 3);
    generate_enemies(EnemyType::Slime, 3);
    generate_enemies(EnemyType::GoblinWitch, 2);
    generate_enemies(EnemyType::Wizard, 1);
    generate_enemies(EnemyType::SkeletonElite, 2);
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
            [added_projectile](const engine::physics::CollisionInfo &collision_info)
            {
                // forward projectile damage through CombatReciever
                CombatReceiver *receiver = dynamic_cast<CombatReceiver *>(collision_info.other.owner());

                if (!receiver)
                    return;

                // Attack only valid if not on bullets cooldown list
                if (added_projectile->can_hit(collision_info.other.owner()))
                {
                    receiver->receive_attack(added_projectile->attack_info());
                    added_projectile->on_entity_collision(collision_info.other.owner());
                }
            });

        added_projectile->set_collision_box(collision_box);
    }
}

engine::core::Vector2 RoomScene::get_shot_direction(int pointer_x, int pointer_y)
{
    const SDL_FPoint mouse_world = camera.screen_to_world(
        static_cast<float>(pointer_x),
        static_cast<float>(pointer_y));

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

void RoomScene::generate_enemies(EnemyType type, std::size_t count)
{
    if (!_room)
        return;

    const EnemyGenerationConfig config{
        .type = type,
        .count = count};

    std::vector<std::unique_ptr<Enemy>> generated_enemies =
        _enemy_generator.generate(*_room, config);

    for (std::unique_ptr<Enemy> &enemy : generated_enemies)
    {
        Enemy *added_enemy = add_object(std::move(enemy));
        if (!added_enemy)
            continue;

        _enemies.push_back(added_enemy);
        physics_manager().register_body(added_enemy, added_enemy, added_enemy);
    }
}
