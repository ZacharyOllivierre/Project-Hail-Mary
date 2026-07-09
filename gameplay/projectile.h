#pragma once

#include "../engine/core/game_object.h"
#include "../engine/core/interface/collidable.h"
#include "../engine/core/interface/kinematic_body.h"
#include "../engine/core/interface/updatable.h"

class Projectile : public GameObject,
                   public Updatable,
                   public Collidable,
                   public KinematicBody
{
public:
    explicit Projectile(
        DepthLayer layer,
        const Vector2 &start_position = Vector2::zero(),
        const Vector2 &start_size = Vector2::zero(),
        const Vector2 &start_velocity = Vector2::zero()) noexcept
        : GameObject(layer), _velocity(start_velocity)
    {
        set_world_rect(Rect(start_position, start_size));
    }

    ~Projectile() override = default;

    void update(double delta) override
    {
        (void)delta;
    }

    [[nodiscard]] Vector2 desired_velocity() const noexcept override
    {
        return _velocity;
    }

    [[nodiscard]] Vector2 velocity() const noexcept
    {
        return _velocity;
    }

    void apply_translation(const Vector2 &delta) noexcept override
    {
        GameObject::set_position(position() + delta);
        _collision_rect.set_position(_collision_rect.position() + delta);
    }

    [[nodiscard]] Rect collision_rect() const noexcept override
    {
        return _collision_rect;
    }

    void on_collision() noexcept override
    {
        destroy();
    }

    void set_velocity(const Vector2 &velocity) noexcept
    {
        _velocity = velocity;
    }

    void set_world_rect(const Rect &rect) noexcept
    {
        GameObject::set_world_rect(rect);
        _collision_rect = rect;
    }

    void set_position(const Vector2 &position) noexcept
    {
        GameObject::set_position(position);
        _collision_rect.set_position(position);
    }

    void set_center(const Vector2 &center) noexcept
    {
        GameObject::set_center(center);
        _collision_rect.set_center(center);
    }

    void set_size(const Vector2 &size) noexcept
    {
        GameObject::set_size(size);
        _collision_rect.set_size(size);
    }

    double age_seconds() const noexcept
    {
        return 0.0;
    }

protected:
    void reset() noexcept override
    {
        GameObject::reset();
    }

private:
    Vector2 _velocity = Vector2::zero();
    Rect _collision_rect{};
};