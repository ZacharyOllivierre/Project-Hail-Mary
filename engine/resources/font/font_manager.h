#pragma once
#include "../resource_sub_manager.h"

#include <SDL_ttf.h>

#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>

using FontPool = std::unordered_map<std::string, TTF_Font*>;

class FontManager : public ResourceSubManager
{
public:
	~FontManager() override;

	bool load_font(
		const std::string& key,
		const std::filesystem::path& file_path,
		int point_size
	);
	bool store_font(const std::string& key, TTF_Font* font);
	TTF_Font* find_font(const std::string_view& key) const;

	void clear() override;
	size_t resource_count() const override;

private:
	FontPool _font_pool;
};
