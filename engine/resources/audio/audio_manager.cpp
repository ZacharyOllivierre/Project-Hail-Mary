#include "audio_manager.h"

bool AudioManager::load_music()
{
	return true;
}

bool AudioManager::load_sound()
{
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
