#include "projectile_manager.h"

#include "attack_info.h"
#include "bullet.h"
#include "projectile.h"

#include "../../engine/physics/collision_manager.h"
#include "../../engine/physics/physics_manager.h"
#include "../../engine/scene/scene.h"
#include "../../engine/tools/logger.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <utility>

void ProjectileManager::bind_scene(engine::scene::Scene& scene,
    engine::physics::PhysicsManager& physics) noexcept
{
    clear();
    _scene_runtime.scene = &scene;
    _scene_runtime.physics = &physics;
}

void ProjectileManager::unbind_scene() noexcept
{
    clear();
    _scene_runtime = {};
}

void ProjectileManager::enqueue_fire_request(ProjectileFireRequest request)
{
    if (!_scene_runtime.is_valid())
    {
        ENGINE_LOG_WARN("ProjectileManager", "Cannot enqueue fire request without a bound scene.");
        return;
    }

    _scheduled_projectiles.reserve(_scheduled_projectiles.size() + request.shots.size());

    for (ShotDescriptor& shot : request.shots)
    {
        if (!std::isfinite(shot.spawn_delay_sec))
        {
            ENGINE_LOG_WARN("ProjectileService", "Fire request contains an invalid shot delay.");
            continue;
        }

        ScheduledProjectile scheduled;
        scheduled.source = request.source;
        scheduled.collision = request.collision;
        scheduled.shot = std::move(shot);
        _scheduled_projectiles.push_back(std::move(scheduled));
    }

    return;
}

void ProjectileManager::update(double delta)
{
    if (!_scene_runtime.is_valid())
        return;

    const float elapsed = static_cast<float>(std::max(0.0, delta));

    // Iterate through all scheduled shots and copy over / spawn ready ones
    for (auto iterator = _scheduled_projectiles.begin();iterator != _scheduled_projectiles.end();)
    {
        if (!iterator->source)
        {
            ENGINE_LOG_WARN("ProjectileManager", "Discarding a queued shot with an invalid source.");
            iterator = _scheduled_projectiles.erase(iterator);
            continue;
        }

        iterator->shot.spawn_delay_sec -= elapsed;
        if (iterator->shot.spawn_delay_sec > 0.0f)
        {
            ++iterator;
            continue;
        }

        ScheduledProjectile scheduled = std::move(*iterator);
        iterator = _scheduled_projectiles.erase(iterator);
        spawn_projectile(std::move(scheduled));
    }
}

void ProjectileManager::clear() noexcept
{
    _scheduled_projectiles.clear();
}

void ProjectileManager::spawn_projectile(ScheduledProjectile scheduled)
{
    if (!_scene_runtime.is_valid() || !scheduled.source)
    {
        ENGINE_LOG_WARN("ProjectileManager", "Cannot spawn projectile from an invalid runtime or source.");
        return;
    }

    // Update bullut spawn position with offset relative to player
    Bullet_Attributes attributes = std::move(scheduled.shot.bullet_attributes);
    attributes.start_position =scheduled.source->center() + scheduled.shot.spawn_offset;

    std::unique_ptr<Projectile> projectile =std::make_unique<Bullet>(attributes);
    Projectile* added_projectile =_scene_runtime.scene->add_object(std::move(projectile));

    if (!added_projectile)
        ENGINE_LOG_ERROR("ProjectileManager", "Bound scene rejected a projectile.");

    if (scheduled.collision.entity_collision_enabled())
    {
        engine::physics::CollisionBox* collision_box =
            engine::physics::CollisionManager::instance()->create_box(
                added_projectile,
                scheduled.collision.layer,
                scheduled.collision.targets,
                [added_projectile](const engine::physics::CollisionInfo& collision_info)
                {
                    // forward projectile damage through CombatReciever
                    CombatReceiver* receiver =dynamic_cast<CombatReceiver*>(collision_info.other.owner());

                    if (!receiver)
                        return;

                    engine::core::GameObject* target = collision_info.other.owner();

                    // Attack only valid if not on bullets cooldown list
                    if (!added_projectile->can_hit(target))
                        return;

                    receiver->receive_attack(added_projectile->attack_info());
                    added_projectile->on_entity_collision(target);
                });

        if (!collision_box)
        {
            ENGINE_LOG_ERROR("ProjectileManager", "Failed to create projectile collision box.");
            added_projectile->destroy();
        }

        added_projectile->set_collision_box(collision_box);
    }

    _scene_runtime.physics->register_body(added_projectile,added_projectile,added_projectile);
}
