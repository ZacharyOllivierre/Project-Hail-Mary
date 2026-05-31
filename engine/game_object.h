#pragma once
#include <SDL.h>
#include "base/vector2.h"
#include "base/depth_layer.h"
#include "input/input_system.h"

class GameObject
{
public:
    explicit GameObject( DepthLayer layer, int order = 0 )
        :_depth_layer(layer), _order_in_layer(order){}

    virtual ~GameObject() = default;

    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;
    GameObject(GameObject&&) = default;
    GameObject& operator=(GameObject&&) = default;

    virtual void on_update(double delta) {}
    virtual void on_render(SDL_Renderer* renderer) {}

    virtual void on_input(const InputSnapshot& input) {}
    virtual void on_input_event(const InputEvent& event) {}

    virtual void reset()
    {
        _visible = true;
        _active = true;
        _destroyed = false;
        _update_when_paused = false;
        _input_when_paused = false;
    }

    void destroy() { _destroyed = true; }
    bool is_destroyed() const { return _destroyed; }

    void set_world_position(const Vector2& position)
    {
        _world_pos = position;
        sync_rect();
    }
    const Vector2& position() const { return _world_pos; }

    void set_center_pos(const Vector2& center_pos)
    {
        _world_pos = { center_pos.x - _size.x * 0.5f, center_pos.y - _size.y * 0.5f };
        sync_rect();
    }

    Vector2 center() const
    {
        return { _world_pos.x + _size.x * 0.5f, _world_pos.y + _size.y * 0.5f };
    }

    const Vector2& size() const { return _size; }
    void set_size(const Vector2& size)
    {
        _size = size;
        sync_rect();
    }

    const SDL_Rect& rect() const { return _obj_rect; }

    DepthLayer depth_layer() const { return _depth_layer; }
    int order_in_layer() const { return _order_in_layer; }

    void set_visible(bool visible) { _visible = visible; }
    bool is_visible() const { return _visible; }

    void set_active(bool active) { _active = active; }
    bool is_active()const { return _active; }

    void set_update_when_paused(bool enable) { _update_when_paused = enable; }
    bool will_update_when_paused() const { return _update_when_paused; }

    void set_input_when_paused(bool enable) { _input_when_paused = enable; }
    bool will_input_when_paused() const { return _input_when_paused; }

private:
    void sync_rect()
    {
        _obj_rect.x = static_cast<int>(_world_pos.x);
        _obj_rect.y = static_cast<int>(_world_pos.y);

        const int width = static_cast<int>(_size.x);
        const int height = static_cast<int>(_size.y);
        _obj_rect.w = width > 0 ? width : 0;
        _obj_rect.h = height > 0 ? height : 0;
    }

    Vector2 _world_pos{ 0 , 0 };
    Vector2 _size { 0 , 0 };

    SDL_Rect _obj_rect{ 0 };

    DepthLayer _depth_layer = DepthLayer::Object;
    int _order_in_layer = 0;

    bool _destroyed = false;
    bool _visible = true;
    bool _active = true;

    bool _update_when_paused = false;
    bool _input_when_paused = false;
};
