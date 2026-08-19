#include "configs_list_loader.h"

#include "../json_loader.h"
#include "../path_manager.h"
#include "../../tools/logger.h"

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
		ENGINE_LOG_ERROR("resource","Load configs list failed: " << result.error);
		return false;
	}

	if (!loader.root().is_object())
	{
		ENGINE_LOG_ERROR("resource","Load configs list failed: root is not an object: "
			<< configs_list_path);
		return false;
	}

	if (!loader.root().contains("configs") || !loader.root().at("configs").is_array())
	{
		ENGINE_LOG_ERROR("resource","Load configs list failed: configs is missing or not an array: "
			<< configs_list_path);
		return false;
	}

	for (const json& item : loader.root().at("configs"))
	{
		if (!item.is_string())
		{
			ENGINE_LOG_ERROR("resource","Load configs list failed: config entry is not a string.");
			return false;
		}

		const std::string config_name = item.get<std::string>();
		const std::filesystem::path config_path =
			PathManager::instance()->resolve_config_path(config_name + ".json");
		if (!std::filesystem::is_regular_file(config_path))
		{
			ENGINE_LOG_ERROR("resource","Load configs list failed: config file does not exist: "
				<< config_path);
			return false;
		}

		out_registry.config_paths[config_name] = config_path;
	}

	return true;
}
}
