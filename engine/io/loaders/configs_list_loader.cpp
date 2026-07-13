#include "configs_list_loader.h"

#include "../json_loader.h"
#include "../path_manager.h"

#include <iostream>
#include <string>

namespace engine::io
{

bool ConfigsListLoader::load(
	const std::filesystem::path& configs_list_path,
	ConfigRegistry& out_registry
) const
{
	out_registry = ConfigRegistry{};

	JsonLoader loader;
	const JsonReadResult result = loader.open_file(configs_list_path);
	if (!result)
	{
		std::cout << "Load configs list failed: " << result.error;
		return false;
	}

	if (!loader.root().is_object())
	{
		std::cout << "Load configs list failed: root is not an object: "
			<< configs_list_path << std::endl;
		return false;
	}

	if (!loader.root().contains("configs") || !loader.root().at("configs").is_array())
	{
		std::cout << "Load configs list failed: configs is missing or not an array: "
			<< configs_list_path << std::endl;
		return false;
	}

	for (const json& item : loader.root().at("configs"))
	{
		if (!item.is_string())
		{
			std::cout << "Load configs list failed: config entry is not a string."
				<< std::endl;
			return false;
		}

		const std::string config_name = item.get<std::string>();
		const std::filesystem::path config_path =
			PathManager::instance()->resolve_config_path(config_name + ".json");
		if (!std::filesystem::is_regular_file(config_path))
		{
			std::cout << "Load configs list failed: config file does not exist: "
				<< config_path << std::endl;
			return false;
		}

		out_registry.config_paths[config_name] = config_path;
	}

	return true;
}
}
