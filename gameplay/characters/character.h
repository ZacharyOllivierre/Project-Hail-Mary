#pragma once
#include "../../engine/animation/animation.h"
#include "../../engine/animation/effect_manager.h"
#include "../../engine/core/game_object.h"
#include "../../engine/core/interface/updatable.h"
#include "../../engine/core/interface/collidable.h"
#include "../../engine/core/interface/kinematic_body.h"

#include "../projectile.h"
#include "../wand.h"

#include <memory>
#include <string>
#include <vector>

class Character : public engine::core::GameObject,public engine::core::Updatable,
				  public engine::core::Collidable,public engine::core::KinematicBody
{
public:
	explicit Character(
		const engine::core::Vector2 &start_position = engine::core::Vector2(300.0f, 300.0f),
		const engine::core::Vector2 &start_size = engine::core::Vector2(100.0f, 100.0f),);
	~Character() override;

	void update(double delta) override;
	void submit_render_commands(std::vector<engine::core::RenderCommand> &out_commands) const override;

	void set_move_speed(float move_speed) noexcept;
	void set_hp(float hp) noexcept;
	void set_character_size(const engine::core::Vector2 &size);
	void set_position(const engine::core::Vector2 &position);

	[[nodiscard]] engine::core::Vector2 desired_velocity() const noexcept override;
	void apply_translation(const engine::core::Vector2 &delta) noexcept override;

	void die();
	void take_damage(float damage) noexcept;

	// getter
	[[nodiscard]] virtual engine::core::Rect collision_rect() const noexcept;
	[[nodiscard]] float move_speed() const noexcept;
	[[nodiscard]] float hp() const noexcept;
	[[nodiscard]] bool is_dead() const noexcept;

private:
	float _move_speed = 240.0f;
	float _hp = 100;

	engine::core::Rect _collision_rect;

	bool _is_dead = false;
};
