#include "audio_manager.h"

AudioManager::~AudioManager()
{
	clear();
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

void AudioManager::clear()
{
	for (SoundPool::value_type& sound : _sound_pool)
	{
		if (sound.second)
			Mix_FreeChunk(sound.second);
	}

	for (MusicPool::value_type& music : _music_pool)
	{
		if (music.second)
			Mix_FreeMusic(music.second);
	}

	_sound_pool.clear();
	_music_pool.clear();
}

size_t AudioManager::resource_count() const
{
	return _sound_pool.size() + _music_pool.size();
}
