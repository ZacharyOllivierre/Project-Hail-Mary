#include "font_manager.h"

#include <iostream>

FontManager::~FontManager()
{
	clear();
}

bool FontManager::load_font(
	const std::string& key,
	const std::filesystem::path& file_path,
	int point_size
)
{
	if (key.empty())
	{
		std::cout << "Load font failed: key is empty." << std::endl;
		return false;
	}

	if (file_path.empty())
	{
		std::cout << "Load font failed: file path is empty: " << key << std::endl;
		return false;
	}

	if (point_size <= 0)
	{
		std::cout << "Load font failed: point size is invalid: " << key << std::endl;
		return false;
	}

	TTF_Font* font = TTF_OpenFont(file_path.string().c_str(), point_size);
	if (!font)
	{
		std::cout << "Load font failed: " << file_path
			<< " error: " << TTF_GetError() << std::endl;
		return false;
	}

	return store_font(key, font);
}

bool FontManager::store_font(const std::string& key, TTF_Font* font)
{
	if (key.empty())
	{
		std::cout << "Store font failed: key is empty." << std::endl;
		if (font)
		{
			TTF_CloseFont(font);
		}
		return false;
	}

	if (!font)
	{
		std::cout << "Store font failed: font is null: " << key << std::endl;
		return false;
	}

	FontPool::iterator iterator = _font_pool.find(key);
	if (iterator != _font_pool.end())
	{
		if (iterator->second)
		{
			TTF_CloseFont(iterator->second);
		}

		iterator->second = font;
		return true;
	}

	_font_pool.emplace(key, font);
	return true;
}

TTF_Font* FontManager::find_font(const std::string_view& key) const
{
	FontPool::const_iterator iterator = _font_pool.find(std::string(key));
	if (iterator == _font_pool.end())
		return nullptr;

	return iterator->second;
}

void FontManager::clear()
{
	for (FontPool::value_type& font : _font_pool)
	{
		if (font.second)
			TTF_CloseFont(font.second);
	}

	_font_pool.clear();
}

size_t FontManager::resource_count() const
{
	return _font_pool.size();
}
