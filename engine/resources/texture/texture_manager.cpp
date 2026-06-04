#include "texture_manager.h"

#include <iostream>
#include <utility>

bool TextureManager::store_texture(const std::string& key, TexturePtr texture)
{
	if (key.empty())
	{
		std::cout << "Store texture failed: key is empty." << std::endl;
		return false;
	}

	if (!texture)
	{
		std::cout << "Store texture failed: texture is null: "
			<< key << std::endl;
		return false;
	}

	if (_texture_pool.contains(key))
		return true;

	_texture_pool.emplace(key, std::move(texture));
	return true;
}

SDL_Texture* TextureManager::find_texture(const std::string_view& key) const
{
	TexturePool::const_iterator iterator = _texture_pool.find(std::string(key));
	if (iterator == _texture_pool.end())
		return nullptr;

	return iterator->second.get();
}

void TextureManager::clear()
{
	_texture_pool.clear();
}

size_t TextureManager::resource_count() const
{
	return _texture_pool.size();
}
