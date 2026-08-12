#pragma once

#include "projectile_fire_request.h"

#include "../../engine/tools/singleton.h"

class ProjectileService final : public engine::tools::Singleton<ProjectileService>
{
    friend class engine::tools::Singleton<ProjectileService>;

public:
    void request_fire(ProjectileFireRequest request);

private:
    ProjectileService() = default;
};
