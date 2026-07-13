#pragma once

#include <SDL_mixer.h>

#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>

namespace engine::resources
{

using SoundPool = std::unordered_map<std::string, Mix_Chunk*>;
using MusicPool = std::unordered_map<std::string, Mix_Music*>;

class AudioManager
{
public:
	bool load_sound(const std::string& key, const std::filesystem::path& file_path);
	bool load_music(const std::string& key, const std::filesystem::path& file_path);

	bool store_sound(const std::string& key, Mix_Chunk* sound);
	bool store_music(const std::string& key, Mix_Music* music);

	Mix_Chunk* find_sound(const std::string_view& key) const;
	Mix_Music* find_music(const std::string_view& key) const;

private:
	SoundPool _sound_pool;
	MusicPool _music_pool;
};
}
