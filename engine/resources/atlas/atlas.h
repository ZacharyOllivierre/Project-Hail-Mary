#pragma once
#include <SDL.h>

#include <filesystem>
#include <initializer_list>
#include <string>
#include <vector>

struct FrameInfo
{
	std::filesystem::path _path;
	SDL_Texture* _texture = nullptr;
	int _width = 0;
	int _height = 0;
	size_t _index = 0;
};

class Atlas
{
public:
	Atlas() = default;
	explicit Atlas(std::string name);
	~Atlas() = default;

	void clear();
	void set_name(std::string name);

	const std::string& name() const;
	bool empty() const;
	size_t size() const;

	bool add_frame(const std::filesystem::path& frame_path, SDL_Texture* texture);
	bool add_texture(SDL_Texture* texture);
	bool add_textures(std::initializer_list<SDL_Texture*> textures);

	const FrameInfo* frame_at(size_t frame_index) const;
	SDL_Texture* texture_at(size_t frame_index) const;

	template<typename... TextureTypes>
	bool add_textures(TextureTypes*... textures)
	{
		if constexpr (sizeof...(textures) == 0)
			return false;

		if (!(... && (textures != nullptr)))
			return false;

		return (... && add_texture(textures));
	}

private:
	std::string _name;
	std::vector<FrameInfo> _frames;
};
