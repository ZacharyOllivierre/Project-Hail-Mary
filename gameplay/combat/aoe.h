//DISCLOSURE : This code file was created with the heavy assistance of AI, specifically ChatGPT. The AI was used to generate code snippets and provide suggestions, which were then reviewed and modified by a human developer. The final implementation reflects a collaborative effort between the AI and the human developer, ensuring that the code meets the desired functionality and quality standards.

#pragma once
#include "../../engine/core/game_object.h"
#include "../../engine/physics/collision_box.h"
#include "../../engine/core/interface/collidable.h"
#include "../../engine/core/interface/updatable.h"
#include "../../engine/animation/animation.h"
#include <memory>




class AreaOfEffect :
    public engine::core::GameObject,
    public engine::core::Updatable
{
public:
    AreaOfEffect(
        const engine::core::Vector2& position,
        const engine::core::Vector2& size,
        float max_duration,
        float damage);

    ~AreaOfEffect() override;

    void update(double delta) override;

    void submit_render_commands(std::vector<engine::core::RenderCommand>& out_commands) const override;

    void set_collision_box(engine::physics::CollisionBox* collision_box) noexcept
    {
        _collision_box = collision_box;
        update_collision_box();
    }
private:

    float _remaining_time;
    float _damage;
    float _damage_cooldown = 0.0f;
    //make const?
    float _damage_interval = 0.25f;

    std::unique_ptr<engine::animation::Animation> _animation;

    engine::core::Vector2 _velocity = engine::core::Vector2::zero();
    engine::core::Rect _collision_rect{};
    engine::core::Rect _damage_rect{};
    engine::physics::CollisionBox* _collision_box = nullptr;
    float _age_seconds = 0.0f;

    void update_collision_box() noexcept
    {
        if (_collision_box)
            _collision_box->set_rect(_damage_rect);
    }
protected:

};