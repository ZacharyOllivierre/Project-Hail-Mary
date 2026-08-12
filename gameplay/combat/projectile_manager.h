#pragma once

#include "projectile_fire_request.h"

#include "../../engine/tools/singleton.h"

#include <vector>

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

class ProjectileService;

class ProjectileManager final : public engine::tools::Singleton<ProjectileManager>
{
    friend class engine::tools::Singleton<ProjectileManager>;
    friend class ProjectileService;

public:
    void bind_scene(engine::scene::Scene& scene, engine::physics::PhysicsManager& physics) noexcept;
    void unbind_scene() noexcept;

    void update(double delta);
    void clear() noexcept;

private:
    struct ScheduledProjectile
    {
        const engine::core::GameObject* source = nullptr;
        ProjectileCollisionProfile collision;
        ShotDescriptor shot;
    };

    ProjectileManager() = default;

    void enqueue_fire_request(ProjectileFireRequest request);
    void spawn_projectile(ScheduledProjectile scheduled);

private:
    ProjectileSceneRuntime _scene_runtime;
    std::vector<ScheduledProjectile> _scheduled_projectiles;
};
