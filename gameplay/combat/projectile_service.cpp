#include "projectile_service.h"
#include "projectile_manager.h"
#include "../../engine/tools/logger.h"

void ProjectileService::request_fire(ProjectileInformation inform, std::vector<ShotDescriptor> shots)
{
	if (!inform.owner)
	{
		ENGINE_LOG_ERROR("ProjectileService", "fire request owner is null");
		return;
	}

	if (inform.layer == engine::physics::CollisionLayer::None)
		ENGINE_LOG_WARN("ProjectileService", "fire request CollisionLayer is None");
	if (inform.targets == engine::physics::CollisionTarget::None)
		ENGINE_LOG_WARN("ProjectileService", "fire request CollisionTarget is None");

	ProjectileManager::instance()->enqueue_fire_request(inform, shots);
}