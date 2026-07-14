#pragma once

#include "character.h"
#include "../../engine/animation/animation.h"

#include <memory>
#include <string>

class Enemy : public Character
{
public:
    Enemy(std::string character_id,
        const engine::core::Vector2& start_position = engine::core::Vector2(300.0f, 300.0f),
        const engine::core::Vector2& start_size = engine::core::Vector2(100.0f, 100.0f));

    void update(double delta) override;
    void submit_render_commands(std::vector<engine::core::RenderCommand>& out_commands) const override;

private:
    std::unique_ptr<engine::animation::Animation> _idle_animation;
};
