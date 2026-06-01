#pragma once
#include "../../thirdparty/nlohmann/json.hpp"

#include <exception>
#include <filesystem>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

using json = nlohmann::json;

struct JsonReadResult
{
    bool success = false;
    std::string error;

    explicit operator bool() const { return success; }
};

class JsonLoader
{
public:
    JsonReadResult open_file(const std::filesystem::path& path);
    void reset();

    bool is_loaded() const;
    const json& root() const;

    template<typename T>
    JsonReadResult get(std::string_view key, T& out) const;

    template<typename T>
    JsonReadResult get(
        const json& node,
        std::string_view key,
        T& out
    ) const;

    template<typename T>
    JsonReadResult get_array(std::string_view key, std::vector<T>& out) const;

    template<typename T>
    JsonReadResult get_array(
        const json& node,
        std::string_view key,
        std::vector<T>& out
    ) const;

    template<typename T>
    T get_or(std::string_view key, T default_value) const;

    template<typename T>
    T get_or(
        const json& node,
        std::string_view key,
        T default_value
    ) const;

    JsonReadResult get_object(
        std::string_view key,
        const json*& out
    ) const;

    JsonReadResult get_object(
        const json& node,
        std::string_view key,
        const json*& out
    ) const;

private:
    static void add_error_message(JsonReadResult& result,
        const std::string& error);

    template<typename T>
    bool read_value(const json& value, T& out) const;

private:
    json _root;
    std::filesystem::path _path;
    bool _loaded = false;
};

template<typename T>
JsonReadResult JsonLoader::get(std::string_view key, T& out) const
{
    JsonReadResult result;

    if (!_loaded)
    {
        add_error_message(result, "JSON is not loaded.");
        return result;
    }

    return get(_root, key, out);
}

template<typename T>
JsonReadResult JsonLoader::get(
    const json& node,
    std::string_view key,
    T& out
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

    if (!read_value(node.at(key_string), out))
    {
        add_error_message(result, "JSON value type mismatch: " + key_string);
        return result;
    }

    result.success = true;
    return result;
}

template<typename T>
JsonReadResult JsonLoader::get_array(std::string_view key, std::vector<T>& out) const
{
    JsonReadResult result;

    if (!_loaded)
    {
        add_error_message(result, "JSON is not loaded.");
        return result;
    }

    return get_array(_root, key, out);
}

template<typename T>
JsonReadResult JsonLoader::get_array(
    const json& node,
    std::string_view key,
    std::vector<T>& out
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

    if (!value.is_array())
    {
        add_error_message(result, "JSON value is not an array: " + key_string);
        return result;
    }

    std::vector<T> values;
    values.reserve(value.size());

    for (const json& item : value)
    {
        T item_value{};
        if (!read_value(item, item_value))
        {
            add_error_message(result, "JSON array item type mismatch: " + key_string);
            return result;
        }
        values.push_back(std::move(item_value));
    }

    out = std::move(values);
    result.success = true;
    return result;
}

template<typename T>
T JsonLoader::get_or(std::string_view key, T default_value) const
{
    T value = default_value;
    return get(key, value) ? value : default_value;
}

template<typename T>
T JsonLoader::get_or(
    const json& node,
    std::string_view key,
    T default_value
) const
{
    T value = default_value;
    return get(node, key, value) ? value : default_value;
}

template<typename T>
bool JsonLoader::read_value(const json& value, T& out) const
{
    try
    {
        if constexpr (std::is_same_v<T, std::filesystem::path>)
        {
            if (!value.is_string())
                return false;

            out = value.get<std::string>();
        }
        else
        {
            out = value.get<T>();
        }
    }
    catch (const std::exception&)
    {
        return false;
    }

    return true;
}
