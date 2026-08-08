#include "collision_effect_behavior.h"

#include "../../bullet.h"
#include "../../../../engine/animation/effect_manager.h"
#include "../../../../engine/scene/scene_manager.h"
#include "../../../scene/room_scene.h"

// Redefinition (here and bullet.cpp)
constexpr double kRadiansToDegrees = 57.29577951308232;

void CollisionEffectBehavior::spawn_effect(BulletBehaviorContext &context)
{
    RoomScene *room_scene = engine::scene::SceneManager::instance()->try_find_scene<RoomScene>();

    if (!room_scene)
        return;

    engine::animation::EffectSpawnRequest request;
    request.effect_key = _effect_key;
    request.position = context.bullet.center();
    request.anchor = engine::animation::EffectAnchor::Center;

    // Angle effect opposite bullet degrees
    engine::core::Vector2 v = context.bullet.desired_velocity();
    request.angle_degrees = std::atan2(v.y, v.x) * kRadiansToDegrees + 180;

    room_scene->spawn_effect(request);
}