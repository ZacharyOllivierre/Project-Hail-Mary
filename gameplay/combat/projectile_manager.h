#pragma once
#include "../../engine/tools/singleton.h"
#include <vector>
#include <utility>

class Projectile;

namespace engine::physics
{
    class PhysicsManager;
}

namespace engine::scene
{
    class Scene;
}

struct ProjectileSceneRuntime
{
    engine::scene::Scene* scene = nullptr;
    engine::physics::PhysicsManager* physics = nullptr;

    [[nodiscard]] bool is_valid() const noexcept
    {
        return scene && physics;
    }
};


class ProjectileManager final : public engine::tools::Singleton<ProjectileManager>
{
    friend class engine::tools::Singleton<ProjectileManager>;

public:
    bool bind_scene(engine::scene::Scene& scene, engine::physics::PhysicsManager& physics) noexcept;
    void unbind_scene() noexcept;

    void enqueue_fire_request(ProjectileInformation inform, std::vector<ShotDescriptor> shots);
    void update(double delta);
    void clear() noexcept;

private:
    void register_projectile(ProjectileInformation inform, std::vector<ShotDescriptor> shots);

private:
    ProjectileSceneRuntime _scene_info;
    std::vector<std::pair<ProjectileInformation, std::vector<ShotDescriptor>>> _scheduled_projectiles;
};