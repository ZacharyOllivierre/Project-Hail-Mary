#pragma once

#include <SDL_mixer.h>

#include <string>
#include <string_view>
#include <unordered_map>

using SoundPool = std::unordered_map<std::string, Mix_Chunk*>;
using MusicPool = std::unordered_map<std::string, Mix_Music*>;

class AudioManager
{
public:
	bool load_music();
	bool load_sound();

	Mix_Chunk* find_sound(const std::string_view& key) const;
	Mix_Music* find_music(const std::string_view& key) const;

private:
	SoundPool _sound_pool;
	MusicPool _music_pool;
};
