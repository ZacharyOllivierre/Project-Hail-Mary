#include "camera.h"

// Init camera with screen size, default position, zoom, and no world bounds
Camera::Camera(int screen_width, int screen_height)
{
    _screen_width = screen_width;
    _screen_height = screen_height;

    _x = 0;
    _y = 0;

    _zoom = 1.0;

    _use_bounds = false;

    _world_width = 0;
    _world_height = 0;
}

// Sets camera's world position and keeps it inside bounds if enabled
void Camera::set_position(float x, float y)
{
    _x = x;
    _y = y;

    clamp_to_bounds();
}

// Moves camera relative to its current position and applies bounds checking
void Camera::move(float dx, float dy)
{
    _x += dx;
    _y += dy;

    clamp_to_bounds();
}

// Changes zoom level prevents zoom values less than min zoom
void Camera::set_zoom(float zoom)
{
    // Prevent invalid zooms (less than 0.1)
    float min_zoom = 0.1;
    if (zoom < min_zoom)
    {
        zoom = min_zoom;
    }

    _zoom = zoom;

    clamp_to_bounds();
}

// Moves camera towards target pos at a rate of smoothing
void Camera::follow(float target_x, float target_y, float smoothing)
{
    _x += (target_x - _x) * smoothing;
    _y += (target_y - _y) * smoothing;

    clamp_to_bounds();
}

void Camera::setBounds(int world_width, int world_height)
{
    _use_bounds = true;

    _world_width = world_width;
    _world_height = world_height;

    clamp_to_bounds();
}

// Converts point from world coordinates to screen coordinates based on camera position and zoom
SDL_FPoint Camera::world_to_screen(float world_x, float world_y) const
{
    SDL_FPoint screen;

    screen.x = (world_x - _x) * _zoom + (_screen_width * 0.5);
    screen.y = (world_y - _y) * _zoom + (_screen_height * 0.5);

    return screen;
}

// Converts a world space rec into screen-space
SDL_FRect Camera::world_to_screen(const SDL_FRect &world_rect) const
{
    SDL_FRect screen_rec;

    screen_rec.x = (world_rect.x - _x) * _zoom + (_screen_width * 0.5);
    screen_rec.y = (world_rect.y - _y) * _zoom + (_screen_height * 0.5);

    screen_rec.w = world_rect.w * _zoom;
    screen_rec.h = world_rect.h * _zoom;

    return screen_rec;
}

// Converts screen pos into world position
SDL_FPoint Camera::screen_to_world(float screen_x, float screen_y) const
{
    SDL_FPoint world;

    world.x = ((screen_x - (_screen_width * 0.5f)) / _zoom) + _x;

    world.y = ((screen_y - (_screen_height * 0.5f)) / _zoom) + _y;

    return world;
}

// Restricts camera position such that all visible area within world boundries
void Camera::clamp_to_bounds()
{
    if (!_use_bounds)
    {
        return;
    }

    float half_width = (_screen_width * 0.5f) / _zoom;

    float half_height = (_screen_height * 0.5f) / _zoom;

    _x = std::clamp(
        _x,
        half_width,
        _world_width - half_width);

    _y = std::clamp(
        _y,
        half_height,
        _world_height - half_height);
}