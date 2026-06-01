#include "json_loader.h"

#include <fstream>
#include <utility>

void JsonLoader::add_error_message(
    JsonReadResult& result,
    const std::string& error
)
{
    result.error += error;
    result.error += '\n';
}

JsonReadResult JsonLoader::open_file(
    const std::filesystem::path& path
)
{
    reset();

    JsonReadResult result;

    if (path.empty())
    {
        add_error_message(result, "JSON path is empty.");
        return result;
    }

    std::ifstream file(path);

    if (!file.is_open())
    {
        add_error_message(result, "JSON open failed: " + path.string());
        return result;
    }

    try
    {
        json parsed;
        file >> parsed;
        _root = std::move(parsed);
    }
    catch (const std::exception& error)
    {
        add_error_message(result, "JSON parse failed: " + path.string());
        add_error_message(result, std::string("Reason: ") + error.what());
        return result;
    }

    _path = path;
    _loaded = true;
    result.success = true;
    return result;
}

void JsonLoader::reset()
{
    _root = json{};
    _path.clear();
    _loaded = false;
}

bool JsonLoader::is_loaded() const
{
    return _loaded;
}

const json& JsonLoader::root() const
{
    return _root;
}

JsonReadResult JsonLoader::get_object(
    std::string_view key,
    const json*& out
) const
{
    JsonReadResult result;

    if (!_loaded)
    {
        add_error_message(result, "JSON is not loaded.");
        return result;
    }

    return get_object(_root, key, out);
}

JsonReadResult JsonLoader::get_object(
    const json& node,
    std::string_view key,
    const json*& out
) const
{
    JsonReadResult result;

    if (!node.is_object())
    {
        add_error_message(result, "JSON node is not an object.");
        return result;
    }

    const std::string key_string(key);

    if (!node.contains(key_string))
    {
        add_error_message(result, "JSON key missing: " + key_string);
        return result;
    }

    const json& value = node.at(key_string);

    if (!value.is_object())
    {
        add_error_message(result, "JSON value is not an object: " + key_string);
        return result;
    }

    out = &value;
    result.success = true;
    return result;
}
