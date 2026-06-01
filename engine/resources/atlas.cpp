#include "atlas.h"

static bool query_texture_size(SDL_Texture* texture, int& width, int& height)
{
	if (!texture)
		return false;

	return SDL_QueryTexture(texture, nullptr, nullptr, &width, &height) == 0;
}

Atlas::Atlas(std::string name)
	: _name(std::move(name))
{
}

void Atlas::clear()
{
	_frames.clear();
}

void Atlas::set_name(std::string name)
{
	_name = std::move(name);
}

const std::string& Atlas::name() const
{
	return _name;
}

bool Atlas::empty() const
{
	return _frames.empty();
}

size_t Atlas::size() const
{
	return _frames.size();
}

bool Atlas::add_frame(const std::filesystem::path& frame_path, SDL_Texture* texture)
{
	int width = 0;
	int height = 0;
	if (!query_texture_size(texture, width, height))
		return false;

	FrameInfo frame_info;
	frame_info._path = frame_path;
	frame_info._texture = texture;
	frame_info._width = width;
	frame_info._height = height;
	frame_info._index = _frames.size();
	_frames.push_back(frame_info);
	return true;
}

bool Atlas::add_texture(SDL_Texture* texture)
{
	return add_frame({}, texture);
}

bool Atlas::add_textures(std::initializer_list<SDL_Texture*> textures)
{
	if (textures.size() == 0)
		return false;

	for (SDL_Texture* texture : textures)
	{
		if (!add_texture(texture))
			return false;
	}

	return true;
}

const FrameInfo* Atlas::frame_at(size_t frame_index) const
{
	if (_frames.empty())
		return nullptr;

	return &_frames[frame_index % _frames.size()];
}

SDL_Texture* Atlas::texture_at(size_t frame_index) const
{
	const FrameInfo* frame_info = frame_at(frame_index);
	return frame_info ? frame_info->_texture : nullptr;
}
