#pragma once

#include "../engine/core/game_object.h"
#include "../engine/core/collision_box.h"
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

    void apply_translation(const Vector2 &delta) noexcept override
    {
        GameObject::set_position(position() + delta);
        _collision_rect.set_position(_collision_rect.position() + delta);
        _damage_rect.set_position(_damage_rect.position() + delta);
        update_collision_box();
    }

    [[nodiscard]] Rect collision_rect() const noexcept override
    {
        return _collision_rect;
    }

    void on_collision(const Vector2 &collision_direction) noexcept override
    {
        destroy();
    }

    void set_velocity(const Vector2 &velocity) noexcept
    {
        _velocity = velocity;
    }

    void set_collision_box(CollisionBox *collision_box) noexcept
    {
        _collision_box = collision_box;
        update_collision_box();
    }

    void set_world_rect(const Rect &rect) noexcept
    {
        GameObject::set_world_rect(rect);
        _collision_rect = rect;
        _damage_rect = rect;
        update_collision_box();
    }

    void set_damage_rect(const Rect &rect) noexcept
    {
        _damage_rect = rect;
        update_collision_box();
    }

    void set_position(const Vector2 &position) noexcept
    {
        GameObject::set_position(position);
        _collision_rect.set_position(position);
        _damage_rect.set_position(position);
        update_collision_box();
    }

    void set_center(const Vector2 &center) noexcept
    {
        GameObject::set_center(center);
        _collision_rect.set_center(center);
        _damage_rect.set_center(center);
        update_collision_box();
    }

    void set_size(const Vector2 &size) noexcept
    {
        GameObject::set_size(size);
        _collision_rect.set_size(size);
        _damage_rect.set_size(size);
        update_collision_box();
    }

    void set_age(const double &age)
    {
        _age_seconds = age;
    }

    double age_seconds() const noexcept
    {
        return _age_seconds;
    }

protected:
    void reset() noexcept override
    {
        GameObject::reset();
    }

private:
    void update_collision_box() noexcept
    {
        if (_collision_box)
            _collision_box->set_rect(_damage_rect);
    }

    Vector2 _velocity = Vector2::zero();
    Rect _collision_rect{};
    Rect _damage_rect{};
    CollisionBox *_collision_box = nullptr;
    double _age_seconds = 0.0;
};
