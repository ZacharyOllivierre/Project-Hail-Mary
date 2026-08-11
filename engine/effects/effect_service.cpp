#include "effect_service.h"

#include "runtime/effect_manager.h"

namespace engine::effects
{

bool EffectService::request_animation_effect(
	const AnimationEffectSpawnRequest& request)
{
	return EffectManager::instance()->dispatch(request);
}

bool  EffectService::request_floating_number_effect(
	const FloatingNumberEffectSpawnRequest& request)
{
	(void)request;

	return false;
}

}
