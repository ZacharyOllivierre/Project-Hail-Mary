#pragma once
#include <SDL_image.h>

#include "../engine/core/game_object.h"
#include "../engine/resources/resource_manager.h"

class TestMap : public GameObject
{
public:
    TestMap();
    ~TestMap() override;

    void submit_render_commands(std::vector<RenderCommand> &out_commands) const override;

private:
    SDL_Texture *_texture = nullptr;
};
