#pragma once

#include "../../engine/animation/effect_manager.h"
#include "../../engine/scene/scene.h"
#include "../../engine/core/geometry/vector2.h"
#include "../character.h"
#include "../map/dungeon_room.h"
#include "room_tile_collision_world.h"

#include <SDL.h>

class RoomScene final : public ::engine::scene::Scene
{
public:
    RoomScene() = default;
    ~RoomScene() override = default;

    void on_enter() override;
    void on_exit() override;
    void reset() override;

    void on_update(double delta) override;
    void on_render(SDL_Renderer *renderer) override;
	//imgui debug
    void on_imgui() override;
    void on_input(const ::engine::input::InputSnapshot &input, const std::vector<::engine::input::InputEvent> &events) override;

    void spawn_effect(const ::engine::animation::EffectSpawnRequest &request);

    ::engine::core::Vector2 closest_enemy_to_point(::engine::core::Vector2 &point);

private:
    void build_room();
    void spawn_player();
    void spawn_enemies();

    Character *_player = nullptr;
    DungeonRoom *_room = nullptr;
    RoomTileCollisionWorld _collision_world;

    vector<Character *> _enemies;
};
