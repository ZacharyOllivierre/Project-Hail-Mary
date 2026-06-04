#include "resource_request_builder.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <utility>

bool ResourceRequestBuilder::append_character_animation_requests(
	const CharacterConfig& character_config,
	const AnimationConfig& animation_config,
	std::vector<AtlasLoadRequest>& atlas_load_requests,
	std::vector<AnimationBuildRequest>& animation_build_requests
) const
{
	if (character_config._id.empty())
	{
		std::cout << "Build resource requests failed: character id is empty." << std::endl;
		return false;
	}

	if (!std::filesystem::is_directory(character_config._texture_root))
	{
		std::cout << "Build resource requests failed: texture root does not exist: "
			<< character_config._texture_root << std::endl;
		return false;
	}

	for (const AnimationClipConfig& clip_config : animation_config._clips)
	{
		if (clip_config._animation_name.empty())
		{
			std::cout << "Build resource requests failed: animation name is empty: "
				<< character_config._id << std::endl;
			return false;
		}

		std::filesystem::path directory_path =
			(character_config._texture_root / clip_config._path).lexically_normal();
		if (!std::filesystem::is_directory(directory_path))
		{
			std::cout << "Build resource requests failed: animation directory does not exist: "
				<< directory_path << std::endl;
			return false;
		}

		std::string animation_key =
			character_config._id + "." + clip_config._animation_name;
		if (clip_config._is_segment)
			animation_key += "." + std::to_string(clip_config._segment_index);

		AtlasLoadRequest atlas_request;
		atlas_request._atlas_key = animation_key;
		atlas_request._directory_path = directory_path;
		atlas_request._frame_count = clip_config._frame_count;

		AnimationBuildRequest animation_request;
		animation_request._animation_key = animation_key;
		animation_request._atlas_key = atlas_request._atlas_key;
		animation_request._fps = clip_config._fps;
		animation_request._loop = clip_config._loop;
		animation_request._segment_index = clip_config._segment_index;

		atlas_load_requests.push_back(std::move(atlas_request));
		animation_build_requests.push_back(std::move(animation_request));
	}

	return true;
}
