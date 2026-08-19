#include "audio_manager.h"
#include "../../tools/logger.h"

namespace engine::resources
{

bool AudioManager::load_sound(
	const std::string& key,
	const std::filesystem::path& file_path
)
{
	if (key.empty())
	{
		ENGINE_LOG_WARN("resource","Load sound failed: key is empty.");
		return false;
	}

	if (file_path.empty())
	{
		ENGINE_LOG_WARN("resource","Load sound failed: file path is empty: " << key);
		return false;
	}

	Mix_Chunk* sound = Mix_LoadWAV(file_path.string().c_str());
	if (!sound)
	{
		ENGINE_LOG_WARN("resource","Load sound failed: " << file_path
			<< " error: " << Mix_GetError());
		return false;
	}

	return store_sound(key, sound);
}

bool AudioManager::load_music(
	const std::string& key,
	const std::filesystem::path& file_path
)
{
	if (key.empty())
	{
		ENGINE_LOG_WARN("resource","Load music failed: key is empty.");
		return false;
	}

	if (file_path.empty())
	{
		ENGINE_LOG_WARN("resource","Load music failed: file path is empty: " << key);
		return false;
	}

	Mix_Music* music = Mix_LoadMUS(file_path.string().c_str());
	if (!music)
	{
		ENGINE_LOG_WARN("resource","Load music failed: " << file_path
			<< " error: " << Mix_GetError());
		return false;
	}

	return store_music(key, music);
}

bool AudioManager::store_sound(const std::string& key, Mix_Chunk* sound)
{
	if (key.empty())
	{
		ENGINE_LOG_WARN("resource","Store sound failed: key is empty.");
		if (sound)
			Mix_FreeChunk(sound);
		return false;
	}

	if (!sound)
	{
		ENGINE_LOG_WARN("resource","Store sound failed: sound is null: " << key);
		return false;
	}

	SoundPool::iterator iterator = _sound_pool.find(key);
	if (iterator != _sound_pool.end())
	{
		if (iterator->second)
			Mix_FreeChunk(iterator->second);

		iterator->second = sound;
		return true;
	}

	_sound_pool.emplace(key, sound);
	return true;
}

bool AudioManager::store_music(const std::string& key, Mix_Music* music)
{
	if (key.empty())
	{
		ENGINE_LOG_WARN("resource","Store music failed: key is empty.");
		if (music)
			Mix_FreeMusic(music);
		return false;
	}

	if (!music)
	{
		ENGINE_LOG_WARN("resource","Store music failed: music is null: " << key);
		return false;
	}

	MusicPool::iterator iterator = _music_pool.find(key);
	if (iterator != _music_pool.end())
	{
		if (iterator->second)
			Mix_FreeMusic(iterator->second);

		iterator->second = music;
		return true;
	}

	_music_pool.emplace(key, music);
	return true;
}


Mix_Chunk* AudioManager::find_sound(const std::string_view& key) const
{
	SoundPool::const_iterator iterator = _sound_pool.find(std::string(key));
	if (iterator == _sound_pool.end())
		return nullptr;

	return iterator->second;
}

Mix_Music* AudioManager::find_music(const std::string_view& key) const
{
	MusicPool::const_iterator iterator = _music_pool.find(std::string(key));
	if (iterator == _music_pool.end())
		return nullptr;

	return iterator->second;
}
}
