#pragma once
#include "../resource_sub_manager.h"

#include <SDL_mixer.h>

#include <string>
#include <string_view>
#include <unordered_map>

using SoundPool = std::unordered_map<std::string, Mix_Chunk*>;
using MusicPool = std::unordered_map<std::string, Mix_Music*>;

class AudioManager : public ResourceSubManager
{
public:
	~AudioManager() override;

	Mix_Chunk* find_sound(const std::string_view& key) const;
	Mix_Music* find_music(const std::string_view& key) const;

	void clear() override;
	size_t resource_count() const override;

private:
	SoundPool _sound_pool;
	MusicPool _music_pool;
};
