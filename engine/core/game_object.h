#pragma once

#include <algorithm>
#include <vector>

#include "depth_layer.h"
#include "geometry/rect.h"
#include "geometry/vector2.h"
#include "render/render_command.h"
#include "../input/input_system.h"

class GameObject
{
public:
    explicit GameObject(DepthLayer layer, int order = 0) noexcept
        : _depth_layer(layer), _order_in_layer(order) {
    }

    virtual ~GameObject() = default;

    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;
    GameObject(GameObject&&) = default;
    GameObject& operator=(GameObject&&) = default;

    virtual void submit_render_commands(std::vector<RenderCommand>& out_commands) const
    {
        (void)out_commands;
    }

    virtual void reset()
    {
        _visible = true;
        _active = true;
        _destroyed = false;
        _time_scale = 1.0;
    }

    void destroy() noexcept { _destroyed = true; }
    [[nodiscard]] bool is_destroyed() const noexcept { return _destroyed; }

    void set_world_rect(const Rect& rect) noexcept { _world_rect = rect; }
    void set_position(const Vector2& position) noexcept { _world_rect.set_position(position); }
    void set_center(const Vector2& center) noexcept { _world_rect.set_center(center); }
    void set_size(const Vector2& size) noexcept { _world_rect.set_size(size); }

    void move_by(const Vector2& offset) noexcept
    {
        _world_rect.set_position(_world_rect.position() + offset);
    }

    [[nodiscard]] const Rect& world_rect() const noexcept { return _world_rect; }
    [[nodiscard]] Vector2 position() const noexcept { return _world_rect.position(); }
    [[nodiscard]] Vector2 center() const noexcept { return _world_rect.center(); }
    [[nodiscard]] Vector2 size() const noexcept { return _world_rect.size(); }

    [[nodiscard]] virtual Rect render_rect() const noexcept { return _world_rect; }
    [[nodiscard]] virtual Rect collision_rect() const noexcept { return _world_rect; }

    [[nodiscard]] DepthLayer depth_layer() const noexcept { return _depth_layer; }
    [[nodiscard]] int order_in_layer() const noexcept { return _order_in_layer; }

    void set_depth_layer(DepthLayer layer) noexcept { _depth_layer = layer; }
    void set_order_in_layer(int order) noexcept { _order_in_layer = order; }

    void set_visible(bool visible) noexcept { _visible = visible; }
    [[nodiscard]] bool is_visible() const noexcept { return _visible; }

    void set_active(bool active) noexcept { _active = active; }
    [[nodiscard]] bool is_active() const noexcept { return _active; }

    void set_time_scale(double scale) noexcept { _time_scale = std::max(0.0, scale); }
    [[nodiscard]] double time_scale() const noexcept { return _time_scale; }
    [[nodiscard]] double scaled_delta(double parent_delta) const noexcept { return parent_delta * _time_scale; }

protected:
    [[nodiscard]] RenderCommand make_render_command() const
    {
        RenderCommand command;
        command._world_rect = render_rect();
        return command;
    }

private:
    Rect _world_rect{};

    DepthLayer _depth_layer = DepthLayer::Item;
    int _order_in_layer = 0;

    bool _destroyed = false;
    bool _visible = true;
    bool _active = true;

    double _time_scale = 1.0;
};