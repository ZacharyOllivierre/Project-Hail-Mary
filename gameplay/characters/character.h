#pragma once

#include "../../engine/core/game_object.h"
#include "../../engine/core/interface/collidable.h"
#include "../../engine/core/interface/kinematic_body.h"
#include "../../engine/core/interface/updatable.h"
#include "../combat/attack_info.h"

class Character : public engine::core::GameObject,public engine::core::Updatable,
                  public engine::core::Collidable, public engine::core::KinematicBody,
                  public CombatReceiver
{
public:
    ~Character() override = default;

    // from GameObject
    // for Scene rendering
    virtual void submit_render_commands(std::vector<engine::core::RenderCommand>& out_commands) const = 0;

    // Character state
    [[nodiscard]] virtual float hp() const noexcept = 0;
    [[nodiscard]] virtual bool is_dead() const noexcept = 0;

    // Character lifecycle
    virtual void die() noexcept = 0;

protected:
    Character() noexcept: engine::core::GameObject(engine::core::DepthLayer::Character){}

    // Character shape
    // Rebuild the PhysicsManager collision rect after a size or pose change.
    virtual void refresh_collision_rect() = 0;
};
