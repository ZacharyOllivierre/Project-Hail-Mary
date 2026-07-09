#pragma once

#include "../../engine/animation/effect_manager.h"
#include "../../engine/scene/scene.h"
#include "../../engine/core/geometry/vector2.h"
#include "../character.h"
#include "../map/dungeon_room.h"
#include "room_tile_collision_world.h"

#include <SDL.h>

class RoomScene final : public Scene
{
public:
    RoomScene() = default;
    ~RoomScene() override = default;

    void on_enter() override;
    void on_exit() override;
    void reset() override;

    void on_update(double delta) override;
    void on_render(SDL_Renderer *renderer) override;
    void on_input(const InputSnapshot &input, const std::vector<InputEvent> &events) override;

    void spawn_effect(const EffectSpawnRequest &request);

private:
    void build_room();
    void spawn_player();

    Character *_player = nullptr;
    DungeonRoom *_room = nullptr;
    RoomTileCollisionWorld _collision_world;
};
