#pragma once
#include <SDL_image.h>

#include "../engine/core/game_object.h"
#include "../engine/core/interface/updatable.h"
#include "../engine/input/contracts/input_snapshot_receiver.h"
#include "../engine/resources/resource_manager.h"

// Todo:
/*
if (!camera.view_rect().intersects(command.world_rect))
    continue;
*/

class Test2Obj : public GameObject
{
public:
    Test2Obj();
    ~Test2Obj();

    void submit_render_commands(std::vector<RenderCommand> &out_commands) const override;

private:
    SDL_Texture *_test = nullptr;
    Vector2 _move_input = Vector2::zero();
    float _move_speed = 240.0f;
};
