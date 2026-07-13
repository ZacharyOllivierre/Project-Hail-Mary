#pragma once

#include "../core/geometry/vector2.h"

#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace engine::resources
{

struct AtlasLoadRequest
{
	std::string atlas_key;
	std::vector<std::filesystem::path> frame_paths;
};

struct AnimationBuildRequest
{
	std::string animation_key;
	std::string atlas_key;
	double fps = 10.0;
	bool loop = true;
	size_t segment_index = 0;
};

struct EffectBuildRequest
{
	std::string effect_key;
	std::string animation_key;
	std::optional<::engine::core::Vector2> default_size;
	std::optional<double> angle_degrees;
};
}
