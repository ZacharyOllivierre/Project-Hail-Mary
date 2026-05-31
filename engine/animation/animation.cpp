#include "animation.h"

Animation::Animation()
{
	_timer.set_one_shot(false);
	_timer.set_wait_time(_interval_seconds);
	_timer.set_on_timeout(
		[this]()
		{
			if (!_atlas || _atlas->empty())
			{
				_frame_index = 0;
				return;
			}

			++_frame_index;
			if (_frame_index < _atlas->size())
				return;

			_frame_index = _is_loop ? 0 : _atlas->size() - 1;
			if (_is_loop)
				return;

			_is_finished = true;
			_timer.pause();
			if (_on_finished)
				_on_finished();
		});
}

void Animation::render(SDL_Renderer* renderer, const SDL_Rect& target_rect, double angle_degrees) const
{
	const FrameInfo* frame_info = current_frame();
	if (!frame_info || !frame_info->_texture)
		return;

	SDL_Rect destination_rect = target_rect;
	SDL_Point center_point{ destination_rect.w / 2, destination_rect.h / 2 };
	SDL_RenderCopyEx(renderer, frame_info->_texture, nullptr, &destination_rect, angle_degrees, &center_point, SDL_FLIP_NONE);
}

void Animation::render(SDL_Renderer* renderer, const SDL_Point& target_position, double angle_degrees) const
{
	const FrameInfo* frame_info = current_frame();
	if (!frame_info || !frame_info->_texture)
		return;

	SDL_Rect destination_rect{};
	destination_rect.x = target_position.x;
	destination_rect.y = target_position.y;
	destination_rect.w = frame_info->_width;
	destination_rect.h = frame_info->_height;

	SDL_Point center_point{ frame_info->_width / 2, frame_info->_height / 2 };
	SDL_RenderCopyEx(renderer, frame_info->_texture, nullptr, &destination_rect, angle_degrees, &center_point, SDL_FLIP_NONE);
}

void Animation::update(double delta_seconds)
{
	if (!_atlas || _atlas->empty() || _is_finished)
		return;

	_timer.update(delta_seconds);
}

void Animation::set_atlas(const Atlas* atlas)
{
	_atlas = atlas;
	reset();
}

void Animation::set_loop(bool is_loop)
{
	_is_loop = is_loop;
}

void Animation::set_interval_seconds(double interval_seconds)
{
	_interval_seconds = interval_seconds > 0.0 ? interval_seconds : 0.1;
	_timer.set_wait_time(_interval_seconds);
}

void Animation::set_on_finished(PlayCallback on_finished)
{
	_on_finished = std::move(on_finished);
}

void Animation::reset()
{
	_timer.restart();
	_frame_index = 0;
	_is_finished = false;
}

void Animation::pause()
{
	_timer.pause();
}

void Animation::resume()
{
	_timer.resume();
}

bool Animation::is_finished() const
{
	return _is_finished;
}

bool Animation::is_paused() const
{
	return _timer.is_paused();
}

size_t Animation::current_frame_index() const
{
	return _frame_index;
}

const FrameInfo* Animation::current_frame() const
{
	if (!_atlas)
		return nullptr;

	return _atlas->frame_at(_frame_index);
}
