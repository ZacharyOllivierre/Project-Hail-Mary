#include "projectile_manager.h"
#include "projectile_service.h"
#include "../../engine/tools/logger.h"

bool ProjectileManager::bind_scene(engine::scene::Scene& scene, engine::physics::PhysicsManager& physics)
{
	clear();
	_scene_info.physics = &physics;
	_scene_info.scene = &scene;

	return _scene_info.is_valid();
}

void ProjectileManager::unbind_scene() noexcept
{
	clear();
	_scene_info = {};
}

void ProjectileManager::enqueue_fire_request(ProjectileInformation inform, std::vector<ShotDescriptor> shots)noexcept
{

}

void ProjectileManager::update(double delta)
{

}

void ProjectileManager::clear() noexcept
{

}