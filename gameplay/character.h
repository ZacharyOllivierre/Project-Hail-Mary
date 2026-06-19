#pragma once

#include "../engine/animation/animation.h"
#include "../engine/core/game_object.h"
#include "../engine/core/interface/updatable.h"
#include "../engine/core/interface/collidable.h"
#include "../engine/core/interface/moveable.h"
#include "../engine/input/contracts/input_snapshot_receiver.h"

#include <functional>
#include <memory>
#include <string>

class Character : public GameObject, 
	public Updatable, public Collidable,public Movable,
	public InputSnapshotReceiver
{
public:
	using EffectSpawnCallback = std::function<void(const std::string&, const Vector2&)>;

	enum class AnimationState
	{
		Idle,
		Move,
		Die
	};

	enum class FacingDirection
	{
		Right,
		Left
	};

	explicit Character(
		std::string character_id,
		const Vector2& start_position = Vector2::zero(),
		const Vector2& start_size = Vector2(100.0f, 100.0f),
		std::string effect_id = {},
		EffectSpawnCallback effect_spawn_callback = {}
	);
	~Character() override;

	void update(double delta) override;
	void on_input_snapshot(const InputSnapshot& input) override;
	void submit_render_commands(std::vector<RenderCommand>& out_commands) const override;

	void set_move_speed(float move_speed) noexcept;
	void set_hp(float hp) noexcept;
	void set_mana(float mana) noexcept;
	void set_character_size(const Vector2& size);
	void set_position(const Vector2& position);

	void move_by(const Vector2& offset)noexcept override;

	void die();
	void take_damage(float damage) noexcept;
	bool use_mana(float mana_cost) noexcept;

	//getter
	[[nodiscard]] virtual Rect collision_rect() const noexcept;
	[[nodiscard]] const std::string& character_id() const noexcept;
	[[nodiscard]] float move_speed() const noexcept;
	[[nodiscard]] float hp() const noexcept;
	[[nodiscard]] float mana() const noexcept;
	[[nodiscard]] AnimationState animation_state() const noexcept;
	[[nodiscard]] FacingDirection facing_direction() const noexcept;
	[[nodiscard]] bool is_dead() const noexcept;

protected:
	[[nodiscard]] std::string animation_key_for(AnimationState state) const;

private:
	std::string _character_id;
	std::string _effect_id;
	EffectSpawnCallback _effect_spawn_callback;
	std::unique_ptr<Animation> _animation;
	AnimationState _animation_state = AnimationState::Idle;
	FacingDirection _facing_direction = FacingDirection::Right;
	Vector2 _move_input = Vector2::zero();


	float _move_speed = 240.0f;
	float _hp = 100;
	float _mana = 100;

	Rect _collision_rect;

	bool _is_dead = false;
};
