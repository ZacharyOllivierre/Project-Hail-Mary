#pragma once

#include "effect_types.h"
#include "../tools/singleton.h"

#define EFFECT_SERVICE (::engine::effects::EffectService::instance())

namespace engine::effects
{

class EffectService final : public engine::tools::Singleton<EffectService>
{
	friend engine::tools::Singleton<EffectService>;

public:
	[[nodiscard]] bool request_animation_effect(
		const AnimationEffectSpawnRequest& request);

private:
	EffectService() = default;
};

}
