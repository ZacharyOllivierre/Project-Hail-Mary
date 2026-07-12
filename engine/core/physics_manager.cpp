#include "physics_manager.h"

#include "game_object.h"
#include "geometry/rect.h"
#include "render/debug_draw.h"

#include <algorithm>
#include <cmath>

namespace
{
    void add_debug_rect(
        const Rect& rect,
        DebugDrawCategory category
    ) noexcept
    {
        DebugDraw::instance()->add_world_rect(rect, category);
    }

    [[nodiscard]] Rect tile_rect(
        const TileCollisionWorld &world,
        int tile_x,
        int tile_y) noexcept
    {
        const Vector2 origin = world.world_origin();
        const Vector2 size = world.tile_size();

        return Rect(
            origin.x + static_cast<float>(tile_x) * size.x,
            origin.y + static_cast<float>(tile_y) * size.y,
            size.x,
            size.y);
    }

    [[nodiscard]] bool is_blocking_tile(
        const TileCollisionWorld &world,
        int tile_x,
        int tile_y) noexcept
    {
        if (tile_x < 0 || tile_x >= world.tile_columns())
            return true;

        if (tile_y < 0 || tile_y >= world.tile_rows())
            return true;

        return world.is_tile_collidable(tile_x, tile_y);
    }

    [[nodiscard]] int tile_index_min(
        float value,
        float origin,
        float tile_extent) noexcept
    {
        return static_cast<int>(std::floor((value - origin) / tile_extent));
    }

    [[nodiscard]] int tile_index_max(
        float value,
        float origin,
        float tile_extent) noexcept
    {
        return static_cast<int>(std::floor((value - origin - Rect::k_epsilon) / tile_extent));
    }

    [[nodiscard]] float resolve_horizontal_move(
        const TileCollisionWorld& world,
        const Rect& current_rect,
        float delta_x
    ) noexcept
    {
        if (std::fabs(delta_x) <= Vector2::k_epsilon)
            return 0.0f;

        const Rect candidate = current_rect.translated(Vector2(delta_x, 0.0f));
        add_debug_rect(candidate, DebugDrawCategory::PhysicsHorizontalCandidate);

        const Vector2 origin = world.world_origin();
        const Vector2 size = world.tile_size();

        const int min_x = tile_index_min(candidate.left(), origin.x, size.x);
        const int max_x = tile_index_max(candidate.right(), origin.x, size.x);
        const int min_y = tile_index_min(candidate.top(), origin.y, size.y);
        const int max_y = tile_index_max(candidate.bottom(), origin.y, size.y);

        float allowed_delta = delta_x;

        for (int tile_y = min_y; tile_y <= max_y; ++tile_y)
        {
            for (int tile_x = min_x; tile_x <= max_x; ++tile_x)
            {
                if (!is_blocking_tile(world, tile_x, tile_y))
                    continue;

                const Rect blocking_tile = tile_rect(world, tile_x, tile_y);
                if (!candidate.intersects(blocking_tile))
                    continue;

                add_debug_rect(blocking_tile, DebugDrawCategory::PhysicsBlockingTile);

                if (delta_x > 0.0f)
                {
                    const float boundary_delta = blocking_tile.left() - current_rect.right();
                    allowed_delta = std::min(allowed_delta, std::max(0.0f, boundary_delta));
                }
                else
                {
                    const float boundary_delta = blocking_tile.right() - current_rect.left();
                    allowed_delta = std::max(allowed_delta, std::min(0.0f, boundary_delta));
                }
            }
        }

        return allowed_delta;
    }

    [[nodiscard]] float resolve_vertical_move(
        const TileCollisionWorld& world,
        const Rect& current_rect,
        float delta_y
    ) noexcept
    {
        if (std::fabs(delta_y) <= Vector2::k_epsilon)
            return 0.0f;

        const Rect candidate = current_rect.translated(Vector2(0.0f, delta_y));
        add_debug_rect(candidate, DebugDrawCategory::PhysicsVerticalCandidate);

        const Vector2 origin = world.world_origin();
        const Vector2 size = world.tile_size();

        const int min_x = tile_index_min(candidate.left(), origin.x, size.x);
        const int max_x = tile_index_max(candidate.right(), origin.x, size.x);
        const int min_y = tile_index_min(candidate.top(), origin.y, size.y);
        const int max_y = tile_index_max(candidate.bottom(), origin.y, size.y);

        float allowed_delta = delta_y;

        for (int tile_y = min_y; tile_y <= max_y; ++tile_y)
        {
            for (int tile_x = min_x; tile_x <= max_x; ++tile_x)
            {
                if (!is_blocking_tile(world, tile_x, tile_y))
                    continue;

                const Rect blocking_tile = tile_rect(world, tile_x, tile_y);
                if (!candidate.intersects(blocking_tile))
                    continue;

                add_debug_rect(blocking_tile, DebugDrawCategory::PhysicsBlockingTile);

                if (delta_y > 0.0f)
                {
                    const float boundary_delta = blocking_tile.top() - current_rect.bottom();
                    allowed_delta = std::min(allowed_delta, std::max(0.0f, boundary_delta));
                }
                else
                {
                    const float boundary_delta = blocking_tile.bottom() - current_rect.top();
                    allowed_delta = std::max(allowed_delta, std::min(0.0f, boundary_delta));
                }
            }
        }

        return allowed_delta;
    }
}

void PhysicsManager::set_collision_world(const TileCollisionWorld *world) noexcept
{
    _collision_world = world;
}

void PhysicsManager::clear_collision_world() noexcept
{
    _collision_world = nullptr;
}

void PhysicsManager::register_body(
    SceneObject *owner,
    KinematicBody *body,
    Collidable *collider) noexcept
{
    if (!owner || !body || !collider)
        return;

    auto existing = std::find_if(_bodies.begin(), _bodies.end(),
                                 [owner](const BodyEntry &entry)
                                 {
                                     return entry.owner == owner;
                                 });

    if (existing != _bodies.end())
    {
        existing->body = body;
        existing->collider = collider;
        return;
    }

    _bodies.push_back(BodyEntry{owner, body, collider});
}

void PhysicsManager::unregister_body(const SceneObject *owner) noexcept
{
    if (!owner)
        return;

    std::erase_if(_bodies, [owner](const BodyEntry &entry)
                  { return entry.owner == owner; });
}

void PhysicsManager::clear_bodies() noexcept
{
    _bodies.clear();
}

void PhysicsManager::step(double delta) noexcept
{
    remove_invalid_entries();

    for (const BodyEntry &entry : _bodies)
    {
        if (!entry.owner || !entry.body || !entry.collider)
            continue;

        if (entry.owner->is_destroyed() || !entry.owner->is_active())
            continue;

        add_debug_rect(
            entry.collider->collision_rect(),
            DebugDrawCategory::PhysicsCollider
        );

        double effective_delta = delta;
        if (const GameObject *game_object = dynamic_cast<const GameObject *>(entry.owner))
        {
            effective_delta = game_object->scaled_delta(delta);
        }

        const Vector2 desired_velocity = entry.body->desired_velocity();
        if (desired_velocity.is_zero() || effective_delta <= 0.0)
            continue;

        const Vector2 desired_move =
            desired_velocity * static_cast<float>(effective_delta);

        if (!entry.collider->collision_enabled() || !_collision_world)
        {
            entry.body->apply_translation(desired_move);
            continue;
        }

        const Vector2 world_tile_size = _collision_world->tile_size();
        if (world_tile_size.x <= Vector2::k_epsilon || world_tile_size.y <= Vector2::k_epsilon)
        {
            entry.body->apply_translation(desired_move);
            continue;
        }

        constexpr int kMaxSubsteps = 128;
        const float max_step_distance =
            std::min(world_tile_size.x, world_tile_size.y) * 0.5f;
        const int substep_count = std::min(
            kMaxSubsteps,
            std::max( 1, static_cast<int>(std::ceil(desired_move.length() / max_step_distance)))
        );
        const Vector2 substep_move = desired_move / static_cast<float>(substep_count);
        bool collided_x = false;
        bool collided_y = false;

        for (int substep = 0; substep < substep_count; ++substep)
        {
            const float allowed_x = resolve_horizontal_move(
                *_collision_world,
                entry.collider->collision_rect(),
                substep_move.x
            );
            collided_x = collided_x
                || std::fabs(allowed_x - substep_move.x) > Vector2::k_epsilon;
            if (std::fabs(allowed_x) > Vector2::k_epsilon)
            {
                entry.body->apply_translation(Vector2(allowed_x, 0.0f));
            }

            const float allowed_y = resolve_vertical_move(
                *_collision_world,
                entry.collider->collision_rect(),
                substep_move.y
            );
            collided_y = collided_y
                || std::fabs(allowed_y - substep_move.y) > Vector2::k_epsilon;
            if (std::fabs(allowed_y) > Vector2::k_epsilon)
            {
                entry.body->apply_translation(Vector2(0.0f, allowed_y));
            }

            add_debug_rect(
                entry.collider->collision_rect(),
                DebugDrawCategory::PhysicsSubstepCollider
            );
        }

        // Added for wand
        // Calc collision direction
        Vector2 collision_direction = Vector2::zero();
        if (collided_x && std::fabs(desired_move.x) > Vector2::k_epsilon)
        {
            if (desired_move.x > 0.0f)
                collision_direction.x = -1.0f;
            else
                collision_direction.x = 1.0f;
        }

        if (collided_y && std::fabs(desired_move.y) > Vector2::k_epsilon)
        {
            if (desired_move.y > 0.0f)
                collision_direction.y = -1.0f;
            else
                collision_direction.y = 1.0f;
        }

        if (collided_x || collided_y)
            entry.collider->on_collision(collision_direction);
        // End of added for wand
    }

    remove_invalid_entries();
}

void PhysicsManager::remove_invalid_entries() noexcept
{
    std::erase_if(_bodies, [](const BodyEntry &entry)
                  { return !entry.owner || !entry.body || !entry.collider || entry.owner->is_destroyed(); });
}
