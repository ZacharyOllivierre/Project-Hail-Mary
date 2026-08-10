#include "effect_service.h"

#include "runtime/effect_manager.h"

namespace engine::effects
{

bool EffectService::request_animation_effect(
	const AnimationEffectSpawnRequest& request)
{
	return EffectManager::instance()->dispatch(request);
}

}
