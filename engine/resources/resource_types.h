#pragma once

#include "../core/geometry/vector2.h"

#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

struct AtlasLoadRequest
{
	std::string _atlas_key;
	std::vector<std::filesystem::path> _frame_paths;
};

struct AnimationBuildRequest
{
	std::string _animation_key;
	double _fps = 10.0;
	bool _loop = true;
	size_t _segment_index = 0;
};

struct EffectBuildRequest
{
	std::string _effect_key;
	std::string _animation_key;
	std::optional<Vector2> _default_size;
	std::optional<double> _angle_degrees;
};
