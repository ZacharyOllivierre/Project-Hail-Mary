#pragma once

#include "asset_config_types.h"

#include <filesystem>

namespace engine::io
{

class ConfigsListLoader
{
public:
	bool load(
		const std::filesystem::path& configs_list_path,
		ConfigRegistry& out_registry
	) const;
};
}
