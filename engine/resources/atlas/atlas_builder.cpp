#include "atlas_builder.h"

#include <iostream>

bool AtlasBuilder::build_atlas(
	const AtlasLoadRequest& request,
	const std::vector<TextureLoadResult>& texture_results,
	Atlas& atlas
) const
{
	if (request._atlas_key.empty())
	{
		std::cout << "Build atlas failed: atlas key is empty." << std::endl;
		return false;
	}

	if (request._frame_count == 0)
	{
		std::cout << "Build atlas failed: frame count is zero: "
			<< request._atlas_key << std::endl;
		return false;
	}

	if (texture_results.size() != request._frame_count)
	{
		std::cout << "Build atlas failed: texture count mismatch: "
			<< request._atlas_key << ", expected " << request._frame_count
			<< ", actual " << texture_results.size() << std::endl;
		return false;
	}

	atlas.clear();
	atlas.set_name(request._atlas_key);

	for (size_t index = 0; index < texture_results.size(); ++index)
	{
		const TextureLoadResult& texture_result = texture_results[index];
		if (!texture_result._success || !texture_result._texture)
		{
			std::cout << "Build atlas failed: texture is invalid: "
				<< request._atlas_key << ", frame " << index << std::endl;
			return false;
		}

		if (texture_result._frame_index != index)
		{
			std::cout << "Build atlas failed: frame index mismatch: "
				<< request._atlas_key << ", expected " << index
				<< ", actual " << texture_result._frame_index << std::endl;
			return false;
		}

		if (!atlas.add_frame(texture_result._frame_path, texture_result._texture.get()))
		{
			std::cout << "Build atlas failed: add frame failed: "
				<< texture_result._frame_path << std::endl;
			return false;
		}
	}

	return true;
}

