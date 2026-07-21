#pragma once

#include "../../engine/core/game_object.h"
#include "../../engine/physics/collision_box.h"
#include "../../engine/core/interface/collidable.h"
#include "../../engine/core/interface/kinematic_body.h"
#include "../../engine/core/interface/updatable.h"

class Projectile : public engine::core::GameObject,
                   public engine::core::Updatable,
                   public engine::core::Collidable,
                   public engine::core::KinematicBody
{
public:
    explicit Projectile(
        engine::core::DepthLayer layer,
        const engine::core::Vector2 &start_position = engine::core::Vector2::zero(),
        const engine::core::Vector2 &start_size = engine::core::Vector2::zero(),
        const engine::core::Vector2 &start_velocity = engine::core::Vector2::zero()) noexcept
        : engine::core::GameObject(layer), _velocity(start_velocity)
    {
        set_world_rect(engine::core::Rect::from_center(start_position, start_size));
    }

    ~Projectile() override = default;

    void update(double delta) override
    {
        (void)delta;
    }

    [[nodiscard]] engine::core::Vector2 desired_velocity() const noexcept override
    {
        return _velocity;
    }

    void apply_translation(const engine::core::Vector2 &delta) noexcept override
    {
        engine::core::GameObject::set_position(position() + delta);
        _collision_rect.set_position(_collision_rect.position() + delta);
        _damage_rect.set_position(_damage_rect.position() + delta);
        update_collision_box();
    }

    [[nodiscard]] engine::core::Rect collision_rect() const noexcept override
    {
        return _collision_rect;
    }

    void on_collision(const engine::core::Vector2 &collision_direction) noexcept override
    {
        SceneObject::destroy();
    }

    virtual void on_entity_collision(GameObject *entity) noexcept
    {
        SceneObject::destroy();
    }

    virtual AttackInfo attack_info() const noexcept = 0;

    // Returns whether or not a bullets collision is valid (cooldown system)
    virtual bool can_hit(GameObject *object) const noexcept = 0;

    void set_velocity(const engine::core::Vector2 &velocity) noexcept
    {
        _velocity = velocity;
    }

    void set_collision_box(engine::physics::CollisionBox *collision_box) noexcept
    {
        _collision_box = collision_box;
        update_collision_box();
    }

    void set_world_rect(const engine::core::Rect &rect) noexcept
    {
        engine::core::GameObject::set_world_rect(rect);
        _collision_rect = rect;
        _damage_rect = rect;
        update_collision_box();
    }

    void set_damage_rect(const engine::core::Rect &rect) noexcept
    {
        _damage_rect = rect;
        update_collision_box();
    }

    void set_position(const engine::core::Vector2 &position) noexcept
    {
        engine::core::GameObject::set_position(position);
        _collision_rect.set_position(position);
        _damage_rect.set_position(position);
        update_collision_box();
    }

    void set_center(const engine::core::Vector2 &center) noexcept
    {
        engine::core::GameObject::set_center(center);
        _collision_rect.set_center(center);
        _damage_rect.set_center(center);
        update_collision_box();
    }

    void set_size(const engine::core::Vector2 &size) noexcept
    {
        engine::core::GameObject::set_size(size);
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
        engine::core::GameObject::reset();
    }

private:
    void update_collision_box() noexcept
    {
        if (_collision_box)
            _collision_box->set_rect(_damage_rect);
    }

    engine::core::Vector2 _velocity = engine::core::Vector2::zero();
    engine::core::Rect _collision_rect{};
    engine::core::Rect _damage_rect{};
    engine::physics::CollisionBox *_collision_box = nullptr;
    double _age_seconds = 0.0;
};
