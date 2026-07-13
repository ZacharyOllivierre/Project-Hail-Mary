#pragma once

#include <SDL.h>
#include <algorithm>

#include "../core/geometry/rect.h"

namespace engine::camera
{

class Camera
{
public:
    Camera(int screen_width, int screen_height);

    void set_position(float x, float y);
    void move(float dx, float dy);

    void set_zoom(float zoom);

    void follow(float target_x, float target_y, float smoothing);

    // Not working
    void setBounds(int world_width, int world_height);

    // Coordinate transormations
    SDL_FPoint world_to_screen(float world_x, float world_y) const;
    SDL_FRect world_to_screen(const SDL_FRect &world_rect) const;
    engine::core::Rect world_to_screen(const engine::core::Rect &world_rect) const;
    SDL_FPoint screen_to_world(float screen_x, float screen_y) const;

    float get_x() const { return _x; }
    float get_y() const { return _y; }

    float get_zoom() const { return _zoom; }

private:
    // Not working probably
    void clamp_to_bounds();

private:
    float _x;
    float _y;

    float _zoom;

    int _screen_width;
    int _screen_height;

    bool _use_bounds;

    int _world_width;
    int _world_height;
};
}
