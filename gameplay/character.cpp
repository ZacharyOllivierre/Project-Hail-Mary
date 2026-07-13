#include "character.h"

#include "../engine/animation/animation_manager.h"
#include "../engine/core/render/render_command.h"
#include "../engine/input/input_state.h"

#include <algorithm>
#include <iostream>
#include <utility>

namespace
{
	constexpr float kCollisionWidthScale = 0.65f;
	constexpr float kCollisionHeightScale = 0.38f;

	[[nodiscard]] engine::core::Rect make_collision_rect(const engine::core::Rect &render_rect) noexcept
	{
		engine::core::Rect collision = engine::core::Rect::zero();
		collision.set_size(engine::core::Vector2(
			render_rect.width() * kCollisionWidthScale,
			render_rect.height() * kCollisionHeightScale));
		collision.set_bottom_center(render_rect.bottom_center());
		return collision;
	}

	void apply_animation_state(
		const std::string &character_id,
		Character::AnimationState new_state,
		std::unique_ptr<engine::animation::Animation> &animation,
		Character::AnimationState &animation_state)
	{
		if (animation && animation_state == new_state)
			return;

		std::string animation_key;
		switch (new_state)
		{
		case Character::AnimationState::Move:
			animation_key = character_id + ".move";
			break;

		case Character::AnimationState::Die:
			animation_key = character_id + ".die";
			break;

		case Character::AnimationState::Idle:
		default:
			animation_key = character_id + ".idle";
			break;
		}

		std::unique_ptr<engine::animation::Animation> new_animation =
			engine::animation::AnimationManager::instance()->create_animation(animation_key);
		if (!new_animation)
		{
			std::cout << "Set Character animation failed: "
					  << animation_key << std::endl;
			return;
		}

		animation = std::move(new_animation);
		animation_state = new_state;
	}
}

Character::Character(
	std::string character_id,
	const engine::core::Vector2 &start_position,
	const engine::core::Vector2 &start_size,
	std::string effect_id)
	: engine::core::GameObject(engine::core::DepthLayer::Character),
	  _character_id(std::move(character_id)),
	  _effect_id(std::move(effect_id))
{
	set_position(start_position);
	set_character_size(start_size);
	apply_animation_state(_character_id, AnimationState::Idle, _animation, _animation_state);
}

Character::~Character() = default;

void Character::update(double delta)
{
	const double frame_delta = scaled_delta(delta);
	if (_animation)
		_animation->update(frame_delta);
}

void Character::on_input_snapshot(const engine::input::InputSnapshot &input)
{
	if (_is_dead)
		return;

	float move_x = 0.0f;
	float move_y = 0.0f;

	if (input.state.is_pressed(engine::input::InputAction::Left))
		move_x -= 1.0f;
	if (input.state.is_pressed(engine::input::InputAction::Right))
		move_x += 1.0f;
	if (input.state.is_pressed(engine::input::InputAction::Up))
		move_y -= 1.0f;
	if (input.state.is_pressed(engine::input::InputAction::Down))
		move_y += 1.0f;

	_move_input = {move_x, move_y};
	_desired_velocity = _move_input.is_zero()
							? engine::core::Vector2::zero()
							: _move_input.normalized() * _move_speed;

	if (_move_input.x < 0.0f)
		_facing_direction = FacingDirection::Left;
	else if (_move_input.x > 0.0f)
		_facing_direction = FacingDirection::Right;

	if (input.state.is_just_pressed(engine::input::InputAction::Attack) &&
		!_effect_id.empty())
	{
		engine::animation::EffectSpawnRequest request;
		request.size = engine::core::Vector2{500, 500};
		request.effect_key = _effect_id;
		request.position = center();
		request.anchor = engine::animation::EffectAnchor::Center;
		request.flip = _facing_direction == FacingDirection::Left
						   ? engine::core::SpriteFlip::Horizontal
						   : engine::core::SpriteFlip::None;
		_pending_effect_requests.push_back(std::move(request));
	}

	apply_animation_state(
		_character_id,
		_move_input.is_zero() ? AnimationState::Idle : AnimationState::Move,
		_animation,
		_animation_state);
}

void Character::submit_render_commands(std::vector<engine::core::RenderCommand> &out_commands) const
{
	if (!_animation)
		return;

	engine::core::RenderCommand command;
	if (_animation->build_render_command(
			world_rect(),
			0.0,
			_facing_direction == FacingDirection::Left
				? engine::core::SpriteFlip::Horizontal
				: engine::core::SpriteFlip::None,
			command))
	{
		out_commands.push_back(std::move(command));
	}
}

std::vector<engine::animation::EffectSpawnRequest> Character::drain_effect_spawn_requests()
{
	std::vector<engine::animation::EffectSpawnRequest> drained_requests = std::move(_pending_effect_requests);
	_pending_effect_requests.clear();
	return drained_requests;
}

std::vector<std::unique_ptr<Projectile>> Character::create_projectile(const engine::core::Vector2 &direction)
{
	return _wand.attack(center(), direction);
}

void Character::set_move_speed(float move_speed) noexcept
{
	_move_speed = std::max(0.0f, move_speed);
	_desired_velocity = _move_input.is_zero()
							? engine::core::Vector2::zero()
							: _move_input.normalized() * _move_speed;
}

void Character::set_hp(float hp) noexcept
{
	_hp = std::max(0.0f, hp);
	if (_hp <= 0.0f)
		die();
}

void Character::set_mana(float mana) noexcept
{
	_mana = std::max(0.0f, mana);
}

void Character::set_character_size(const engine::core::Vector2 &size)
{
	engine::core::GameObject::set_size(size);
	_collision_rect = make_collision_rect(world_rect());
}

void Character::set_position(const engine::core::Vector2 &position)
{
	engine::core::GameObject::set_position(position);
	_collision_rect = make_collision_rect(world_rect());
}

engine::core::Vector2 Character::desired_velocity() const noexcept
{
	return _desired_velocity;
}

void Character::apply_translation(const engine::core::Vector2 &delta) noexcept
{
	engine::core::GameObject::set_position(position() + delta);
	_collision_rect.set_position(_collision_rect.position() + delta);
}

void Character::die()
{
	if (_is_dead)
		return;

	_is_dead = true;
	_move_input = engine::core::Vector2::zero();
	_desired_velocity = engine::core::Vector2::zero();
	apply_animation_state(_character_id, AnimationState::Die, _animation, _animation_state);
}

void Character::take_damage(float damage) noexcept
{
	if (_is_dead || damage <= 0.0f)
		return;

	set_hp(_hp - damage);
}

bool Character::use_mana(float mana_cost) noexcept
{
	if (mana_cost <= 0.0f)
		return true;

	if (_mana < mana_cost)
		return false;

	_mana -= mana_cost;
	return true;
}

engine::core::Rect Character::collision_rect() const noexcept
{
	return _collision_rect;
}

const std::string &Character::character_id() const noexcept
{
	return _character_id;
}

float Character::move_speed() const noexcept
{
	return _move_speed;
}

float Character::hp() const noexcept
{
	return _hp;
}

float Character::mana() const noexcept
{
	return _mana;
}

Character::AnimationState Character::animation_state() const noexcept
{
	return _animation_state;
}

Character::FacingDirection Character::facing_direction() const noexcept
{
	return _facing_direction;
}

bool Character::is_dead() const noexcept
{
	return _is_dead;
}

std::string Character::animation_key_for(AnimationState state) const
{
	switch (state)
	{
	case AnimationState::Move:
		return _character_id + ".move";

	case AnimationState::Die:
		return _character_id + ".die";

	case AnimationState::Idle:
	default:
		return _character_id + ".idle";
	}
}
