#include "font_manager.h"
#include "../../tools/logger.h"

namespace engine::resources
{


bool FontManager::load_font(const std::string& key,const std::filesystem::path& file_path,int point_size)
{
	if (key.empty())
	{
		ENGINE_LOG_WARN("resource","Load font failed: key is empty.");
		return false;
	}

	if (file_path.empty())
	{
		ENGINE_LOG_WARN("resource","Load font failed: file path is empty: " << key);
		return false;
	}

	if (point_size <= 0)
	{
		ENGINE_LOG_WARN("resource","Load font failed: point size is invalid: " << key);
		return false;
	}

	TTF_Font* font = TTF_OpenFont(file_path.string().c_str(), point_size);
	if (!font)
	{
		ENGINE_LOG_WARN("resource","Load font failed: " << file_path
			<< " error: " << TTF_GetError());
		return false;
	}

	return store_font(key, font);
}

bool FontManager::store_font(const std::string& key, TTF_Font* font)
{
	if (key.empty())
	{
		ENGINE_LOG_WARN("resource","Store font failed: key is empty.");
		if (font)
		{
			TTF_CloseFont(font);
		}
		return false;
	}

	if (!font)
	{
		ENGINE_LOG_WARN("resource","Store font failed: font is null: " << key);
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
}
