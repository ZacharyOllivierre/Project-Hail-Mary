#include "projectile_service.h"

#include "projectile_manager.h"
#include "../../engine/tools/logger.h"

#include <cmath>
#include <utility>

void ProjectileService::request_fire(ProjectileFireRequest request)
{
    if (!request.source)
    {
        ENGINE_LOG_ERROR("ProjectileService", "Fire request source is null.");
        return;
    }

    if (request.source->is_destroyed())
    {
        ENGINE_LOG_ERROR("ProjectileService", "Fire request source is destroyed.");
        return;
    }

    if (request.shots.empty())
    {
        ENGINE_LOG_WARN("ProjectileService", "Fire request contains no shots.");
        return;
    }

    if (!request.collision.is_complete())
        ENGINE_LOG_WARN("ProjectileService", "Fire request collision profile is incomplete.");

    ProjectileManager::instance()->enqueue_fire_request(std::move(request));
}
