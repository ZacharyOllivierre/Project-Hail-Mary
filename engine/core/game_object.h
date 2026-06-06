#pragma once
#include <algorithm>
#include <vector>

#include "scene_object.h"
#include "depth_layer.h"
#include "geometry/rect.h"
#include "geometry/vector2.h"

struct RenderCommand;

class GameObject : public SceneObject
{
public:
    // Higher depth layers and order values are rendered closer to the viewer.
    explicit GameObject(DepthLayer layer, int order = 0) noexcept
        : _depth_layer(layer), _order_in_layer(order) {}

    virtual ~GameObject() = default;

    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;
    GameObject(GameObject&&) = delete;
    GameObject& operator=(GameObject&&) = delete;

    virtual void submit_render_commands(std::vector<RenderCommand>& out_commands) const
    {
        (void)out_commands;
    }

    virtual void reset() noexcept override
    {
        SceneObject::reset();
        _time_scale = 1.0;
    }

    void set_world_rect(const Rect& rect) noexcept { _world_rect = rect; }
    void set_position(const Vector2& position) noexcept { _world_rect.set_position(position); }
    void set_center(const Vector2& center) noexcept { _world_rect.set_center(center); }
    void set_size(const Vector2& size) noexcept { _world_rect.set_size(size); }

    [[nodiscard]] const Rect& world_rect() const noexcept { return _world_rect; }
    [[nodiscard]] Vector2 position() const noexcept { return _world_rect.position(); }
    [[nodiscard]] Vector2 center() const noexcept { return _world_rect.center(); }
    [[nodiscard]] Vector2 size() const noexcept { return _world_rect.size(); }

    [[nodiscard]] virtual Rect render_rect() const noexcept { return _world_rect; }

    [[nodiscard]] DepthLayer depth_layer() const noexcept { return _depth_layer; }
    [[nodiscard]] int order_in_layer() const noexcept { return _order_in_layer; }

    void set_time_scale(double scale) noexcept { _time_scale = std::max(0.0, scale); }
    [[nodiscard]] double time_scale() const noexcept { return _time_scale; }
    [[nodiscard]] double scaled_delta(double parent_delta) const noexcept { return parent_delta * _time_scale; }


    [[nodiscard]] bool update_when_paused() const override { return _update_when_paused; }
    [[nodiscard]] bool receive_input_when_paused() const override{    return _receive_input_when_paused;}

    void set_update_when_paused(bool enabled) {    _update_when_paused = enabled;}
    void set_receive_input_when_paused(bool enabled) { _receive_input_when_paused = enabled;}

private:
    Rect _world_rect{};

    DepthLayer _depth_layer = DepthLayer::Item;
    int _order_in_layer = 0;

    bool _update_when_paused = false;
    bool _receive_input_when_paused = false;

    double _time_scale = 1.0;
};