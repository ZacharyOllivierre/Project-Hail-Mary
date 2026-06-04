#pragma once

#include <SDL_ttf.h>

#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>

using FontPool = std::unordered_map<std::string, TTF_Font*>;

class FontManager
{
public:
	bool load_font(const std::string& key,const std::filesystem::path& file_path,int point_size);
	bool store_font(const std::string& key, TTF_Font* font);
	TTF_Font* find_font(const std::string_view& key) const;

private:
	FontPool _font_pool;
};
