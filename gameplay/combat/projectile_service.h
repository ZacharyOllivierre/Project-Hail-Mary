#pragma once

#include "../../engine/tools/singleton.h"
#include "../../engine/core/game_object.h"
#include "../../engine/physics/collision_manager.h"

#include "wand_types.h"

#include <expected>
#include <memory>
#include <vector>

struct ProjectileFireError
{

};

struct ProjectileInformation
{
    engine::core::GameObject* owner = nullptr;
    engine::physics::CollisionLayer layer = engine::physics::CollisionLayer::None;
    engine::physics::CollisionTarget targets = engine::physics::CollisionTarget::None;
    engine::physics::CollisionCallback on_collided;
};

class ProjectileService final : public engine::tools::Singleton<ProjectileService>
{
    friend class engine::tools::Singleton<ProjectileService>;

public:
    void request_fire(ProjectileInformation inform,std::vector<ShotDescriptor> shots);

private:
    ProjectileService() = default;
};