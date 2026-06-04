#pragma once

#include <cstddef>
#include <filesystem>
#include <string>

struct AtlasLoadRequest
{
	std::string _atlas_key;
	std::filesystem::path _directory_path;
	size_t _frame_count = 0;
};

struct AnimationBuildRequest
{
	std::string _animation_key;
	std::string _atlas_key;
	double _fps = 10.0;
	bool _loop = true;
	size_t _segment_index = 0;
};

struct EffectBuildRequest
{
	std::string _effect_key;
	std::string _animation_key;
};
