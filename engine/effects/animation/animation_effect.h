#pragma once

#include "../../animation/animation.h"
#include "../../core/game_object.h"
#include "../../core/interface/updatable.h"

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace engine::effects
{

class AnimationEffect : public engine::core::GameObject, public engine::core::Updatable
{
public:
	using Callback = std::function<void(AnimationEffect&)>;

	AnimationEffect(
		std::string effect_key,
		std::string animation_key,
		std::unique_ptr<engine::animation::Animation> animation
	);
	~AnimationEffect() override = default;

	void submit_render_commands(
		std::vector<engine::core::RenderCommand>& out_commands) const override;
	void update(double delta) override;

	[[nodiscard]] std::unique_ptr<AnimationEffect> clone() const;

	void set_angle(double angle_degrees) noexcept;
	void set_flip(engine::core::SpriteFlip flip) noexcept;
	void set_start_delay(double delay_seconds) noexcept;
	void set_on_started(Callback callback);
	void set_on_finished(Callback callback);
	void schedule_callback(double delay_seconds, Callback callback);

	[[nodiscard]] bool is_started() const noexcept;

private:
	struct ScheduledCallback
	{
		double execute_at_seconds = 0.0;
		std::size_t sequence = 0;
		Callback callback;
	};

	void start_playback();
	void invoke_due_callbacks();
	void cancel_scheduled_callbacks() noexcept;

	std::string _effect_key;
	std::string _animation_key;
	double _angle_degrees = 0.0;
	engine::core::SpriteFlip _flip = engine::core::SpriteFlip::None;
	std::unique_ptr<engine::animation::Animation> _animation;
	double _start_delay_remaining_seconds = 0.0;
	double _playback_elapsed_seconds = 0.0;
	bool _started = false;
	bool _finished_callback_invoked = false;
	std::size_t _next_callback_sequence = 0;
	Callback _on_started;
	Callback _on_finished;
	std::vector<ScheduledCallback> _scheduled_callbacks;
};

}
