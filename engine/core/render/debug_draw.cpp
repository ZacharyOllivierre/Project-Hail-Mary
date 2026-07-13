#include "debug_draw.h"

#include "render_command.h"
#include "sdl_render_command_executor.h"

namespace engine::core
{

void DebugDraw::set_enabled(bool enabled) noexcept
{
    _enabled = enabled;
    if (!_enabled)
        clear();
}

bool DebugDraw::enabled() const noexcept
{
    return _enabled;
}

void DebugDraw::begin_frame() noexcept
{
    clear();
}

void DebugDraw::clear() noexcept
{
    _world_rects.clear();
}

void DebugDraw::add_world_rect(
    const Rect& world_rect,
    DebugDrawCategory category) noexcept
{
    if (!_enabled)
        return;

    _world_rects.push_back(WorldRect{ world_rect, category });
}

void DebugDraw::render(SDL_Renderer* renderer, const engine::camera::Camera& camera) const
{
    if (!renderer || !_enabled || _world_rects.empty())
        return;

    std::vector<UiRenderCommand> commands;
    commands.reserve(_world_rects.size());

    for (const WorldRect& world_rect : _world_rects)
    {
        commands.push_back(make_ui_draw_rect_command(
            camera.world_to_screen(world_rect.rect),
            color_for(world_rect.category)));
    }

    execute_render_commands(renderer, commands);
}

SDL_Color DebugDraw::color_for(DebugDrawCategory category) noexcept
{
    switch (category)
    {
    case DebugDrawCategory::PhysicsCollider:
        return SDL_Color{ 0, 255, 80, 255 };
    case DebugDrawCategory::PhysicsSubstepCollider:
        return SDL_Color{ 140, 255, 180, 255 };
    case DebugDrawCategory::PhysicsHorizontalCandidate:
        return SDL_Color{ 255, 220, 0, 255 };
    case DebugDrawCategory::PhysicsVerticalCandidate:
        return SDL_Color{ 0, 220, 255, 255 };
    case DebugDrawCategory::PhysicsBlockingTile:
        return SDL_Color{ 255, 40, 40, 255 };
    case DebugDrawCategory::CollisionWorld:
        return SDL_Color{ 160, 160, 160, 255 };
    case DebugDrawCategory::CollisionPlayer:
        return SDL_Color{ 80, 160, 255, 255 };
    case DebugDrawCategory::CollisionEnemy:
        return SDL_Color{ 255, 80, 80, 255 };
    case DebugDrawCategory::CollisionPlayerProjectile:
        return SDL_Color{ 80, 255, 80, 255 };
    case DebugDrawCategory::CollisionEnemyProjectile:
        return SDL_Color{ 255, 150, 50, 255 };
    case DebugDrawCategory::CollisionHit:
    default:
        return SDL_Color{ 255, 255, 255, 255 };
    }
}
}
