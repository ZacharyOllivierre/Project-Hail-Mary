#pragma once

#include "../core/geometry/rect.h"
#include "../core/render/render_command.h"
#include "../resources/atlas/atlas.h"
#include "../tools/timer.h"

#include <functional>

namespace engine::animation
{

class Animation
{
public:
	using PlayCallback = std::function<void()>;

	Animation();
	virtual ~Animation() = default;

	void update(double delta_seconds);

	[[nodiscard]] bool build_render_command(
		const engine::core::Rect& target_rect,
		double angle_degrees,
		engine::core::SpriteFlip flip,
		engine::core::RenderCommand& out_command
	) const;

	void set_atlas(const engine::resources::Atlas* atlas);
	void set_loop(bool is_loop);
	void set_interval_seconds(double interval_seconds);
	void set_on_finished(PlayCallback on_finished);

	void reset();
	void pause();
	void resume();

	bool is_finished() const;
	bool is_paused() const;
	size_t current_frame_index() const;
	const engine::resources::FrameInfo* current_frame() const;

protected:
	double _interval_seconds = 0.1;
	bool _is_loop = true;
	bool _is_finished = false;
	size_t _frame_index = 0;
	engine::tools::Timer _timer;
	PlayCallback _on_finished;
	const engine::resources::Atlas* _atlas = nullptr;
};
}
