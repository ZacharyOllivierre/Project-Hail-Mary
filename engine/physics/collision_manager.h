#pragma once

#include "collision_box.h"
#include "../tools/singleton.h"

#include <memory>
#include <vector>

namespace engine::physics
{

class CollisionManager final : public engine::tools::Singleton<CollisionManager>
{
    friend class engine::tools::Singleton<CollisionManager>;

public:
    CollisionBox* create_box(engine::core::GameObject* owner,CollisionLayer layer,
        CollisionTarget targets,CollisionCallback on_collided);
    void destroy_box(CollisionBox* collision_box) noexcept;
    void clear() noexcept;

    void update();

private:
    CollisionManager() = default;

    [[nodiscard]] bool is_box_valid(const CollisionBox* collision_box) const noexcept;
    [[nodiscard]] bool is_pending_destruction(const CollisionBox* collision_box) const noexcept;
    [[nodiscard]] bool contains_box(const CollisionBox* collision_box) const noexcept;
    void remove_invalid_boxes() noexcept;
    void apply_pending_changes() noexcept;

private:
    std::vector<std::unique_ptr<CollisionBox>> _collision_boxes;
    std::vector<std::unique_ptr<CollisionBox>> _pending_boxes;
    std::vector<CollisionBox*> _pending_destructions;
    bool _is_updating = false;
    bool _clear_pending = false;
};
}
