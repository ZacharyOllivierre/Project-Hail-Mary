//DISCLOSURE : This code file was created with the heavy assistance of AI, specifically ChatGPT. The AI was used to generate code snippets and provide suggestions, which were then reviewed and modified by a human developer. The final implementation reflects a collaborative effort between the AI and the human developer, ensuring that the code meets the desired functionality and quality standards.

#include "aoe.h"

#include "../../engine/physics/collision_manager.h"
#include "../../engine/physics/collision_box.h"
#include "../../gameplay/combat/attack_info.h"
#include "../../engine/animation/animation_manager.h"
#include "../../engine/core/render/render_command.h"
#include "../../engine/tools/logger.h"

#include <utility>
//TODO
//Tested via having it set to where an explosion should be created at cursor when an attack is inputted - animation is not centered and collision box not created correctly

AreaOfEffect::AreaOfEffect(
    //requires in-world position, size, how long it'll last, and damage
    //Probably will remove damage when creating different types of AOE
    const engine::core::Vector2& position, const engine::core::Vector2& size,
    float max_duration, float damage)
    : GameObject(engine::core::DepthLayer::Item),
    _remaining_time(max_duration),
    _damage(damage)
{
    set_center(position);
    set_size(size);

    _animation = //using impact radial for testing purposes
        engine::animation::AnimationManager::instance()->create_animation("effect.fire.impact_radial");

    _collision_box = //still trying to figure how your boxs and colliders work.....
        engine::physics::CollisionManager::instance()->create_box(
            this,
            engine::physics::CollisionLayer::Player,
            engine::physics::CollisionTarget::Enemy,
            [this](const engine::physics::CollisionInfo& info)

            {
                if (_damage_cooldown > 0.0f)
                {
                    return;
                }
                engine::core::GameObject* target = info.other.owner();
                CombatReceiver* receiver = dynamic_cast<CombatReceiver*>(info.other.owner());

                if (!receiver)
                {
                    return;
                }

                AttackInfo attack;
                attack.base_damage = _damage;

                receiver->receive_attack(attack);

                _damage_cooldown = _damage_interval;
            }
        );
}


AreaOfEffect::~AreaOfEffect() = default;

void AreaOfEffect::update(double delta)
{
    const float delt = static_cast<float>(delta);

    _remaining_time -= delt;

    if (_remaining_time <= 0.0f)
    {
        destroy();
        return;
    }

    if (_damage_cooldown > 0.0f)
    {
        _damage_cooldown -= delt;
    }

    if (_animation)
        _animation->update(scaled_delta(delta));
}

void AreaOfEffect::submit_render_commands(
    std::vector<engine::core::RenderCommand>& out_commands) const
{
    if (!_animation)
        return;

    engine::core::RenderCommand command;

    if (_animation->build_render_command(
        world_rect(),
        0.0,
        engine::core::SpriteFlip::None,
        command))
    {
        out_commands.push_back(std::move(command));
    }
}
