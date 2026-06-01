#pragma once

#include <functional>

class Timer
{
public:
	using TimeoutCallback = std::function<void()>;

	Timer() = default;
	~Timer() = default;

	void restart();
	void set_wait_time(double wait_time_seconds);
	void set_one_shot(bool is_one_shot);
	void set_on_timeout(TimeoutCallback on_timeout);
	void pause();
	void resume();

	bool is_paused() const;
	void update(double delta_seconds);

private:
	double _elapsed_seconds = 0.0;
	double _wait_time_seconds = 0.0;
	bool _is_paused = false;
	bool _has_shot = false;
	bool _is_one_shot = false;
	TimeoutCallback _on_timeout;
	unsigned long long _state_version = 0;
};
