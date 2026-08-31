#pragma once

#include "audio_settings.h"
#include "sound_playback_scheduler.h"
#include "sound_playback_types.h"
#include "../tools/singleton.h"

#include <array>
#include <string_view>

#define AUDIO_SERVICE (::engine::audio::AudioService::instance())

namespace engine::audio
{

class AudioService : public engine::tools::Singleton<AudioService>
{
	friend engine::tools::Singleton<AudioService>;

public:
	bool init(const AudioSettings& settings);
	void shutdown();

	bool play_sound(std::string_view key, int loops = 0);
	SoundRequestResult request_sound(std::string_view key,const SoundPlayOptions& options = {});
	void update(double delta_seconds);
	bool stop_sound(SoundHandle handle);
	void cancel_all_scheduled_sounds();

	bool set_sound_group_config(
		SoundGroup group,
		const SoundGroupConfig& config);
	const SoundGroupConfig& sound_group_config(SoundGroup group) const;
	void set_sound_group_volume(SoundGroup group, int volume);
	[[nodiscard]] int sound_group_volume(SoundGroup group) const;

	bool play_music(std::string_view key, int loops = -1);
	void stop_music();
	void stop_all_sounds();

	void set_master_volume(int volume);
	void set_music_volume(int volume);
	void set_sound_volume(int volume);

	const AudioSettings& settings() const;

private:
	int start_sound(std::string_view key, int loops, SoundGroup group);
	void apply_volumes();
	void apply_sound_group_volume(SoundGroup group);
	void apply_sound_channel_volume(int channel, SoundGroup group) const;
	static int clamp_volume(int volume);
	static int to_mix_volume(int volume);

	AudioSettings _settings{};
	SoundPlaybackScheduler _sound_scheduler;
	std::array<int, kSoundGroupCount> _sound_group_volumes{ 100, 100, 100, 100 };
	bool _initialized = false;
};

}
