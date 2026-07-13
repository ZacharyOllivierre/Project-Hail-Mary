#pragma once

#include "../../camera/camera.h"
#include "../geometry/rect.h"
#include "../../tools/singleton.h"

#include <SDL.h>

#include <vector>

namespace engine::core
{

enum class DebugDrawCategory
{
    PhysicsCollider,
    PhysicsSubstepCollider,
    PhysicsHorizontalCandidate,
    PhysicsVerticalCandidate,
    PhysicsBlockingTile,
    CollisionWorld,
    CollisionPlayer,
    CollisionEnemy,
    CollisionPlayerProjectile,
    CollisionEnemyProjectile,
    CollisionHit
};

class DebugDraw final : public engine::tools::Singleton<DebugDraw>
{
    friend class engine::tools::Singleton<DebugDraw>;

public:
    void set_enabled(bool enabled) noexcept;
    [[nodiscard]] bool enabled() const noexcept;

    void begin_frame() noexcept;
    void clear() noexcept;
    void add_world_rect(
        const Rect& world_rect,
        DebugDrawCategory category) noexcept;
    void render(SDL_Renderer* renderer, const engine::camera::Camera& camera) const;

private:
    struct WorldRect
    {
        Rect rect{};
        DebugDrawCategory category = DebugDrawCategory::PhysicsCollider;
    };

    DebugDraw() = default;

    [[nodiscard]] static SDL_Color color_for(DebugDrawCategory category) noexcept;

private:
    bool _enabled = false;
    std::vector<WorldRect> _world_rects;
};
}
