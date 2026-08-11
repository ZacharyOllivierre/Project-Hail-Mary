#include "collision_effect_behavior.h"

#include "../../bullet.h"
#include "../../../../engine/effects/effect_service.h"

// Redefinition (here and bullet.cpp)
constexpr double kRadiansToDegrees = 57.29577951308232;

void CollisionEffectBehavior::spawn_effect(BulletBehaviorContext &context)
{
	engine::effects::AnimationEffectSpawnRequest request;
	request.effect_key = _effect_key;
	request.position = context.bullet.center();
	request.anchor = engine::effects::EffectAnchor::Center;

    // Angle effect opposite bullet degrees
    engine::core::Vector2 v = context.bullet.desired_velocity();
    request.angle_degrees = std::atan2(v.y, v.x) * kRadiansToDegrees + 180;

	if (!EFFECT_SERVICE->request_animation_effect(request))
	{
		//someting
	}
}
