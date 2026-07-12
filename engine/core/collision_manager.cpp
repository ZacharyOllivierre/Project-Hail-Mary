#include "collision_manager.h"

#include "game_object.h"
#include "render/debug_draw.h"

#include <algorithm>

namespace
{
    [[nodiscard]] DebugDrawCategory collision_debug_category(
        CollisionLayer layer) noexcept
    {
        switch (layer)
        {
        case CollisionLayer::World:
            return DebugDrawCategory::CollisionWorld;
        case CollisionLayer::Player:
            return DebugDrawCategory::CollisionPlayer;
        case CollisionLayer::Enemy:
            return DebugDrawCategory::CollisionEnemy;
        case CollisionLayer::PlayerProjectile:
            return DebugDrawCategory::CollisionPlayerProjectile;
        case CollisionLayer::EnemyProjectile:
            return DebugDrawCategory::CollisionEnemyProjectile;
        case CollisionLayer::None:
        default:
            return DebugDrawCategory::CollisionWorld;
        }
    }
}

CollisionBox* CollisionManager::create_box(GameObject* owner,CollisionLayer layer,
    CollisionTarget targets,CollisionCallback on_collided)
{
    if (!owner)
        return nullptr;

    std::unique_ptr<CollisionBox> collision_box(new CollisionBox(
        owner,layer,targets,std::move(on_collided)));
    CollisionBox* raw_collision_box = collision_box.get();

    if (_is_updating)
        _pending_boxes.push_back(std::move(collision_box));
    else
        _collision_boxes.push_back(std::move(collision_box));

    return raw_collision_box;
}

void CollisionManager::destroy_box(CollisionBox* collision_box) noexcept
{
    if (!collision_box || !contains_box(collision_box))
        return;

    if (_is_updating)
    {
        if (!is_pending_destruction(collision_box))
            _pending_destructions.push_back(collision_box);
        return;
    }

    std::erase_if(_collision_boxes,
        [collision_box](const std::unique_ptr<CollisionBox>& candidate)
        {return candidate.get() == collision_box;});
    std::erase_if( _pending_boxes,
        [collision_box](const std::unique_ptr<CollisionBox>& candidate)
        {return candidate.get() == collision_box;});
}

void CollisionManager::clear() noexcept
{
    if (_is_updating)
    {
        _clear_pending = true;
        return;
    }

    _collision_boxes.clear();
    _pending_boxes.clear();
    _pending_destructions.clear();
}

void CollisionManager::update()
{
    remove_invalid_boxes();
    _is_updating = true;

    for (const std::unique_ptr<CollisionBox>& collision_box : _collision_boxes)
    {
        if (!is_box_valid(collision_box.get()))
            continue;

        DebugDraw::instance()->add_world_rect(
            collision_box->rect(),
            collision_debug_category(collision_box->layer()));
    }

    for (size_t left_index = 0; left_index < _collision_boxes.size(); ++left_index)
    {
        CollisionBox* left = _collision_boxes[left_index].get();
        if (!is_box_valid(left))
            continue;

        for (size_t right_index = left_index + 1;
             right_index < _collision_boxes.size();
             ++right_index)
        {
            CollisionBox* right = _collision_boxes[right_index].get();
            if (!is_box_valid(right) || left->owner() == right->owner())
                continue;

            if (!left->can_collide_with(*right)
                || !left->rect().intersects(right->rect()))
            {
                continue;
            }

            DebugDraw::instance()->add_world_rect(
                left->rect(),
                DebugDrawCategory::CollisionHit);
            DebugDraw::instance()->add_world_rect(
                right->rect(),
                DebugDrawCategory::CollisionHit);
            left->invoke_on_collided(*right);
            right->invoke_on_collided(*left);
        }
    }

    _is_updating = false;
    apply_pending_changes();
    remove_invalid_boxes();
}

bool CollisionManager::is_box_valid(const CollisionBox* collision_box) const noexcept
{
    if (!collision_box || _clear_pending || is_pending_destruction(collision_box))
        return false;

    const GameObject* owner = collision_box->owner();
    return owner && !owner->is_destroyed() && owner->is_active();
}

bool CollisionManager::is_pending_destruction(const CollisionBox* collision_box) const noexcept
{
    return std::find(_pending_destructions.begin(), _pending_destructions.end(),
    collision_box)!= _pending_destructions.end();
}

bool CollisionManager::contains_box(const CollisionBox* collision_box) const noexcept
{
    const auto contains = [collision_box](
                              const std::vector<std::unique_ptr<CollisionBox>>& boxes)
    {
        return std::any_of(
            boxes.begin(),boxes.end(),
            [collision_box](const std::unique_ptr<CollisionBox>& candidate)
            { return candidate.get() == collision_box;});
    };

    return contains(_collision_boxes) || contains(_pending_boxes);
}

void CollisionManager::remove_invalid_boxes() noexcept
{
    std::erase_if(_collision_boxes,
        [this](const std::unique_ptr<CollisionBox>& collision_box)
        { 
               return !is_box_valid(collision_box.get());
        });
}

void CollisionManager::apply_pending_changes() noexcept
{
    if (_clear_pending)
    {
        _collision_boxes.clear();
        _pending_boxes.clear();
        _pending_destructions.clear();
        _clear_pending = false;
        return;
    }

    if (!_pending_destructions.empty())
    {
        std::erase_if(
            _collision_boxes,
            [this](const std::unique_ptr<CollisionBox>& collision_box)
            {
                return is_pending_destruction(collision_box.get());
            });
    }

    for (std::unique_ptr<CollisionBox>& collision_box : _pending_boxes)
    {
        if (!is_pending_destruction(collision_box.get()))
            _collision_boxes.push_back(std::move(collision_box));
    }
    _pending_boxes.clear();
    _pending_destructions.clear();
}
