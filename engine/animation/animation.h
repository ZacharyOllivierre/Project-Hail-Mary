#pragma once
#include "../tools/timer.h"
#include "../resources/atlas/atlas.h"

#include <SDL.h>

#include <functional>

class Animation
{
public:
	using PlayCallback = std::function<void()>;

	Animation();
	virtual ~Animation() = default;

	virtual void render(SDL_Renderer* renderer, const SDL_Rect& target_rect, double angle_degrees) const;
	virtual void render(SDL_Renderer* renderer, const SDL_Point& target_position, double angle_degrees) const;
	virtual void update(double delta_seconds);

	void set_atlas(const Atlas* atlas);
	void set_loop(bool is_loop);
	void set_interval_seconds(double interval_seconds);
	void set_on_finished(PlayCallback on_finished);

	void reset();
	void pause();
	void resume();

	bool is_finished() const;
	bool is_paused() const;
	size_t current_frame_index() const;
	const FrameInfo* current_frame() const;

protected:
	double _interval_seconds = 0.1;
	bool _is_loop = true;
	bool _is_finished = false;
	size_t _frame_index = 0;
	Timer _timer;
	PlayCallback _on_finished;
	const Atlas* _atlas = nullptr;
};
